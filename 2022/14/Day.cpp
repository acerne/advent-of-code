#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>
#include <numeric>

namespace aoc {

struct Point {
    int32_t x{0};
    int32_t y{0};

    friend Point operator+(const Point& lhs, const Point& rhs) {
        return Point{lhs.x + rhs.x, lhs.y + rhs.y};
    }

    friend Point operator-(const Point& lhs, const Point& rhs) {
        return Point{lhs.x - rhs.x, lhs.y - rhs.y};
    }
};

class Cave {
    static constexpr Point DOWN{0, 1};
    static constexpr Point LEFT{-1, 0};
    static constexpr Point RIGHT{1, 0};

  public:
    Cave(const Input& input, bool infinite_floor = false) {
        int32_t x_min{std::numeric_limits<int32_t>::max()};
        int32_t x_max{std::numeric_limits<int32_t>::min()};
        int32_t y_min{std::numeric_limits<int32_t>::max()};
        int32_t y_max{std::numeric_limits<int32_t>::min()};

        for (const auto& points : input) {
            for (const auto& point : points) {
                x_min = std::min(point.x, x_min);
                x_max = std::max(point.x, x_max);
                y_min = std::min(point.y, y_min);
                y_max = std::max(point.y, y_max);
            }
        }

        const int expansion = infinite_floor ? 200 : 3;
        x_min -= expansion;
        x_max += expansion;
        y_min = std::min(y_min, 0);
        y_max += 2;

        m_offset_x = x_min;
        m_offset_y = y_min;

        m_ceiling = std::vector<std::vector<char>>(y_max - y_min + 1,
                                                   std::vector<char>(x_max - x_min + 1, '.'));
        m_ceiling.back() = std::vector<char>(x_max - x_min + 1, infinite_floor ? '#' : 'x');

        for (const auto& points : input) {
            for (int i = 1; i < points.size(); i++) {
                const Point& a = points.at(i - 1);
                const Point& b = points.at(i);
                for (int y = std::min(a.y, b.y); y <= std::max(a.y, b.y); y++) {
                    for (int x = std::min(a.x, b.x); x <= std::max(a.x, b.x); x++) {
                        m_ceiling.at(y - y_min).at(x - x_min) = '#';
                    }
                }
            }
        }
    }

  public:
    bool dropGrainOfSand(Point sand) {
        size_t n_grains{0};
        sand.x -= m_offset_x;
        sand.y -= m_offset_y;

        if (at(sand) == 'o') {
            // Part 2 exit condition
            return false;
        }

        Point landed = recursiveFall(sand);
        if (at(landed) == 'x') {
            // Part 1 exit condition
            return false;
        } else {
            m_ceiling.at(landed.y).at(landed.x) = 'o';
            return true;
        }
    }

    size_t grainCount() const {
        size_t count{0};
        for (const auto& row : m_ceiling) {
            count += std::count_if(row.begin(), row.end(), [](char c) { return c == 'o'; });
        }
        return count;
    }

    void print() const {
        std::stringstream ss;
        for (const auto& row : m_ceiling) {
            for (char c : row) {
                ss << c;
            }
            ss << std::endl;
        }
        std::cout << ss.str() << std::endl;
    }

  private:
    char at(const Point& p) const {
        return m_ceiling.at(p.y).at(p.x);
    }

    Point recursiveFall(Point sand) const {
        if (at(sand + DOWN) == 'x') {
            return sand + DOWN;
        } else if (at(sand + DOWN) != '.') {
            if (at(sand + LEFT + DOWN) == '.') {
                return recursiveFall(sand + LEFT + DOWN);
            } else if (at(sand + RIGHT + DOWN) == '.') {
                return recursiveFall(sand + RIGHT + DOWN);
            } else {
                return sand;
            }
        } else {
            return recursiveFall(sand + DOWN);
        }
    }

  private:
    std::vector<std::vector<char>> m_ceiling;
    int32_t m_offset_x;
    int32_t m_offset_y;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        std::vector<Point> points;
        for (const auto& token : helpers::tokenize(line, " -> ")) {
            const auto [x, y] = helpers::split(token, ',');
            points.push_back({std::stoi(x), std::stoi(y)});
        }
        inputs.push_back(points);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Cave cave(input);
    while (cave.dropGrainOfSand({500, 0})) {
        // cave.print();
    }
    return cave.grainCount();
}

Result Day::partTwo(const Input& input) const {
    Cave cave(input, true);
    size_t i = 0;
    while (cave.dropGrainOfSand({500, 0})) {
        // if (i % 20 == 0) {
        //     cave.print();
        // }
    }
    return cave.grainCount();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc