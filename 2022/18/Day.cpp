#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <deque>
#include <iostream>
#include <set>

namespace aoc {

struct Cube {
    int32_t x;
    int32_t y;
    int32_t z;
};

struct Range {
    int32_t x{0};
    int32_t y{0};
    int32_t z{0};
};

class Volume {
    using Row = std::vector<uint8_t>;
    using Slice = std::vector<Row>;

    static constexpr uint8_t INSIDE_AIR = 0;
    static constexpr uint8_t OUTSIDE_AIR = 127;
    static constexpr uint8_t LAVA = 255;

  public:
    Volume(const Input& input) {
        for (const auto& cube : input) {
            m_range.x = std::max(m_range.x, cube.x);
            m_range.y = std::max(m_range.y, cube.y);
            m_range.z = std::max(m_range.z, cube.z);
        }
        m_range.x++;
        m_range.y++;
        m_range.z++;

        // Create volume of INSIDE_AIR
        m_data = std::vector<Slice>(m_range.z, Slice(m_range.y, Row(m_range.x, INSIDE_AIR)));

        // Add LAVA to volume
        for (const auto& cube : input) {
            this->at(cube.x, cube.y, cube.z) = LAVA;
        }

        // Change INSIDE_AIR to OUTSIDE_AIR where applicable
        // Top and bottom are outside
        for (int y = 0; y < m_range.y; y++) {
            for (int x = 0; x < m_range.x; x++) {
                if (this->at(x, y, 0) == INSIDE_AIR) {
                    this->at(x, y, 0) = OUTSIDE_AIR;
                }
                if (this->at(x, y, m_range.z - 1) == INSIDE_AIR) {
                    this->at(x, y, m_range.z - 1) = OUTSIDE_AIR;
                }
            }
        }
        // Front and back are outside
        for (int z = 0; z < m_range.z; z++) {
            for (int x = 0; x < m_range.x; x++) {
                if (this->at(x, 0, z) == INSIDE_AIR) {
                    this->at(x, 0, z) = OUTSIDE_AIR;
                }
                if (this->at(x, m_range.y - 1, z) == INSIDE_AIR) {
                    this->at(x, m_range.y - 1, z) = OUTSIDE_AIR;
                }
            }
        }
        // Left and right are outside
        for (int z = 0; z < m_range.z; z++) {
            for (int y = 0; y < m_range.y; y++) {
                if (this->at(0, y, z) == INSIDE_AIR) {
                    this->at(0, y, z) = OUTSIDE_AIR;
                }
                if (this->at(m_range.x - 1, y, z) == INSIDE_AIR) {
                    this->at(m_range.x - 1, y, z) = OUTSIDE_AIR;
                }
            }
        }
        // Mark the rest of the voxels that are outside
        bool changed;
        do {
            changed = false;
            for (int z = 1; z < m_range.z - 1; z++) {
                for (int y = 1; y < m_range.y - 1; y++) {
                    for (int x = 1; x < m_range.x - 1; x++) {
                        if (this->at(x, y, z) == INSIDE_AIR) {
                            if (this->at(x, y, z - 1) == OUTSIDE_AIR
                                || this->at(x, y, z + 1) == OUTSIDE_AIR
                                || this->at(x, y - 1, z) == OUTSIDE_AIR
                                || this->at(x, y + 1, z) == OUTSIDE_AIR
                                || this->at(x - 1, y, z) == OUTSIDE_AIR
                                || this->at(x + 1, y, z) == OUTSIDE_AIR) {
                                this->at(x, y, z) = OUTSIDE_AIR;
                                changed = true;
                            }
                        }
                    }
                }
            }
        } while (changed);
    }

  public:
    size_t countSides() const {  // Part 1
        size_t sides{0};
        for (int z = 0; z < m_range.z; z++) {
            for (int y = 0; y < m_range.y; y++) {
                for (int x = 0; x < m_range.x; x++) {
                    if (this->at(x, y, z) == LAVA) {
                        if (z == 0 || this->at(x, y, z - 1) != LAVA) {
                            sides++;
                        }
                        if (y == 0 || this->at(x, y - 1, z) != LAVA) {
                            sides++;
                        }
                        if (x == 0 || this->at(x - 1, y, z) != LAVA) {
                            sides++;
                        }
                        if (z == m_range.z - 1 || this->at(x, y, z + 1) != LAVA) {
                            sides++;
                        }
                        if (y == m_range.y - 1 || this->at(x, y + 1, z) != LAVA) {
                            sides++;
                        }
                        if (x == m_range.x - 1 || this->at(x + 1, y, z) != LAVA) {
                            sides++;
                        }
                    }
                }
            }
        }
        return sides;
    }

    size_t countOutsideSides() const {  // Part 2
        size_t sides{0};
        for (int z = 0; z < m_range.z; z++) {
            for (int y = 0; y < m_range.y; y++) {
                for (int x = 0; x < m_range.x; x++) {
                    if (this->at(x, y, z) == LAVA) {
                        if (z == 0 || this->at(x, y, z - 1) == OUTSIDE_AIR) {
                            sides++;
                        }
                        if (y == 0 || this->at(x, y - 1, z) == OUTSIDE_AIR) {
                            sides++;
                        }
                        if (x == 0 || this->at(x - 1, y, z) == OUTSIDE_AIR) {
                            sides++;
                        }
                        if (z == m_range.z - 1 || this->at(x, y, z + 1) == OUTSIDE_AIR) {
                            sides++;
                        }
                        if (y == m_range.y - 1 || this->at(x, y + 1, z) == OUTSIDE_AIR) {
                            sides++;
                        }
                        if (x == m_range.x - 1 || this->at(x + 1, y, z) == OUTSIDE_AIR) {
                            sides++;
                        }
                    }
                }
            }
        }
        return sides;
    }

  private:
    uint8_t at(int x, int y, int z) const {
        return m_data.at(z).at(y).at(x);
    }

    uint8_t& at(int x, int y, int z) {
        return m_data.at(z).at(y).at(x);
    }

  private:
    std::vector<Slice> m_data;
    Range m_range;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        const auto tokens = helpers::tokenize(line, ',');
        inputs.push_back(
            {std::stoi(tokens.at(0)), std::stoi(tokens.at(1)), std::stoi(tokens.at(2))});
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Volume volume(input);
    size_t n_sides = volume.countSides();
    return n_sides;
}

Result Day::partTwo(const Input& input) const {
    Volume volume(input);
    size_t n_sides = volume.countOutsideSides();
    return n_sides;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc