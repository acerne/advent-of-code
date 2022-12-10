#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

namespace aoc {

bool overlaps(int32_t a1, int32_t a2, int32_t b1, int32_t b2) {
    return (a1 <= b1 && b1 <= a2) || (b1 <= a1 && a1 <= b2);
}

struct Cuboid {
    int32_t x1, x2;
    int32_t y1, y2;
    int32_t z1, z2;

    bool intersects(const Cuboid& other) const {
        return overlaps(x1, x2, other.x1, other.x2) && overlaps(y1, y2, other.y1, other.y2)
               && overlaps(z1, z2, other.z1, other.z2);
    }

    Cuboid intersection(const Cuboid& other) const {
        return Cuboid{.x1 = std::max(x1, other.x1),
                      .x2 = std::min(x2, other.x2),
                      .y1 = std::max(y1, other.y1),
                      .y2 = std::min(y2, other.y2),
                      .z1 = std::max(z1, other.z1),
                      .z2 = std::min(z2, other.z2)};
    }

    size_t area() const {
        // Prone to overflow if area is calculated as int32_t one-liner
        size_t dx = x2 - x1 + 1;
        size_t dy = y2 - y1 + 1;
        size_t dz = z2 - z1 + 1;
        return dx * dy * dz;
    }
};

template <int32_t RANGE = std::numeric_limits<int32_t>::max()>
class ReactorCore {
  public:
    ReactorCore() = default;

  public:
    void execute(const Instruction& step) {
        if (step.x1 < -RANGE || step.x2 > RANGE || step.y1 < -RANGE || step.y2 > RANGE
            || step.z1 < -RANGE || step.z2 > RANGE) {
            return;
        }

        const Cuboid cuboid{.x1 = step.x1,
                            .x2 = step.x2,
                            .y1 = step.y1,
                            .y2 = step.y2,
                            .z1 = step.z1,
                            .z2 = step.z2};

        std::vector<std::pair<bool, Cuboid>> new_cuboids;
        if (step.on) {
            new_cuboids.push_back({true, cuboid});
        }
        for (const auto& [is_on, existing] : m_core) {
            if (cuboid.intersects(existing)) {
                new_cuboids.push_back({!is_on, cuboid.intersection(existing)});
            }
        }
        for (const auto& item : new_cuboids) {
            m_core.push_back(item);
        }
    }

    size_t activeCubeCount() const {
        size_t turned_on = 0;
        size_t turned_off = 0;
        for (const auto& [flag, cuboid] : m_core) {
            if (flag) {
                turned_on += cuboid.area();
            } else {
                turned_off += cuboid.area();
            }
        }
        return turned_on - turned_off;
    }

  private:
    std::vector<std::pair<bool, Cuboid>> m_core;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        const auto [state, remainder] = helpers::split(line, ' ');
        const auto data = helpers::tokenize(remainder, ',');
        const auto [x1, x2] = helpers::split(data.at(0).substr(2), "..");
        const auto [y1, y2] = helpers::split(data.at(1).substr(2), "..");
        const auto [z1, z2] = helpers::split(data.at(2).substr(2), "..");
        inputs.push_back(Instruction{
            .on = state == "on" ? true : false,
            .x1 = std::stoi(x1),
            .x2 = std::stoi(x2),
            .y1 = std::stoi(y1),
            .y2 = std::stoi(y2),
            .z1 = std::stoi(z1),
            .z2 = std::stoi(z2),
        });
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    ReactorCore<50> core;
    for (const auto& step : input) {
        core.execute(step);
    }
    return core.activeCubeCount();
}

Result Day::partTwo(const Input& input) const {
    ReactorCore core;
    for (const auto& step : input) {
        core.execute(step);
    }
    return core.activeCubeCount();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc