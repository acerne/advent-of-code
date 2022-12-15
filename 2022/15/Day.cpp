#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>
#include <optional>
#include <set>

namespace aoc {

struct Point {
    int32_t x{0};
    int32_t y{0};

    size_t manhattan(const Point& p) const {
        return abs(x - p.x) + abs(y - p.y);
    }

    bool operator==(const Point& rhs) const {
        return std::tie(x, y) == std::tie(rhs.x, rhs.y);
    }

    bool operator<(const Point& rhs) const {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }

    friend Point operator+(const Point& lhs, const Point& rhs) {
        return Point{lhs.x + rhs.x, lhs.y + rhs.y};
    }

    friend Point operator-(const Point& lhs, const Point& rhs) {
        return Point{lhs.x - rhs.x, lhs.y - rhs.y};
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "( " << p.x << ", " << p.y << " )";
    }
};

struct Range {
    int32_t x1{0};
    int32_t y1{0};
    int32_t x2{0};
    int32_t y2{0};

    void expand(const Point& p, int32_t d) {
        x1 = std::min(std::min(x1, p.x + d), p.x - d);
        x2 = std::max(std::max(x2, p.x + d), p.x - d);
        y1 = std::min(std::min(y1, p.y + d), p.y - d);
        y2 = std::max(std::max(y2, p.y + d), p.y - d);
    }

    bool isInside(const Point& p) const {
        return p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2;
    }
};

class SearchZone {
  public:
    SearchZone(const Input& input) {
        for (const auto& [sensor, beacon] : input) {
            size_t d = sensor.manhattan(beacon);
            m_sensors[sensor] = d;
            m_beacons.insert(beacon);
            m_range.expand(sensor, d);
        }
    }

    size_t countRowCandidate(int row) {  // Part 1
        size_t count{0};
        for (Point p{.x = m_range.x1, .y = row}; p.x <= m_range.x2; p.x++) {
            if (m_beacons.contains(p)) {
                continue;
            }
            if (m_sensors.contains(p)) {
                continue;
            }
            for (const auto& [sensor, d] : m_sensors) {
                if (p.manhattan(sensor) <= d) {
                    count++;
                    break;
                }
            }
        }
        return count;
    }

    size_t findDistressBeacon(const Range& range) {  // Part 2
        // Search the d+1 surroundings of each sensor
        for (const auto& [sensor, d] : m_sensors) {
            for (int dx = 0; dx <= d + 1; dx++) {
                int dy = d + 1 - dx;  // dx + dy == d+1
                std::array<Point, 4> points = {
                    sensor + Point{dx,  dy },
                      sensor + Point{dx,  -dy},
                      sensor + Point{-dx, dy },
                    sensor + Point{-dx, -dy}
                };

                for (const auto& p : points) {
                    if (range.isInside(p)) {
                        bool is_in_sensor_range{false};
                        for (const auto& [sensor, d] : m_sensors) {
                            int last{-1};
                            if (p.manhattan(sensor) <= d) {
                                is_in_sensor_range = true;
                                break;
                            }
                        }
                        if (!is_in_sensor_range) {
                            // The only possible beacon location found
                            return static_cast<size_t>(p.x) * size_t(4000000)
                                   + static_cast<size_t>(p.y);
                        }
                    }
                }
            }
        }
        return 0;
    }

    void print() const {
        std::stringstream ss;
        Point p;
        for (p.y = m_range.y1; p.y <= m_range.y2; p.y++) {
            for (p.x = m_range.x1; p.x <= m_range.x2; p.x++) {
                char spot = '.';
                for (const auto& beacon : m_beacons) {
                    if (p == beacon) {
                        spot = 'B';
                        break;
                    }
                }
                if (spot == '.') {
                    for (const auto& [sensor, d] : m_sensors) {
                        if (p == sensor) {
                            spot = 'S';
                            break;
                        }
                    }
                }
                if (spot == '.') {
                    for (const auto& [sensor, d] : m_sensors) {
                        if (p.manhattan(sensor) <= d) {
                            spot = '#';
                            break;
                        }
                    }
                }
                ss << spot;
            }
            ss << std::endl;
        }
        std::cout << ss.str() << std::endl;
    }

  private:
    std::map<Point, size_t> m_sensors;
    std::set<Point> m_beacons;
    Range m_range;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        const auto tokens = helpers::tokenize(line, ' ');
        Point sensor{.x = std::stoi(tokens.at(2).substr(2, tokens.at(2).size() - 2)),
                     .y = std::stoi(tokens.at(3).substr(2, tokens.at(3).size() - 2))};
        Point beacon{.x = std::stoi(tokens.at(8).substr(2, tokens.at(8).size() - 2)),
                     .y = std::stoi(tokens.at(9).substr(2, tokens.at(9).size() - 1))};
        inputs.push_back({sensor, beacon});
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    SearchZone zone(input);
    // zone.print();
    if (input.size() == 14) {
        // Sample requires different parameters
        return zone.countRowCandidate(10);
    } else {
        return zone.countRowCandidate(2000000);
    }
}

Result Day::partTwo(const Input& input) const {
    SearchZone zone(input);
    // zone.print();
    if (input.size() == 14) {
        // Sample requires different parameters
        return zone.findDistressBeacon(Range{0, 0, 20, 20});
    } else {
        return zone.findDistressBeacon(Range{0, 0, 4000000, 4000000});
    }
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc