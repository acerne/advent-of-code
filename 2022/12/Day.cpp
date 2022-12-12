#include "Day.h"

#include "FactoryManager.h"
#include "console.h"
#include "helpers.h"

#include <cmath>
#include <iostream>
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

struct Map {
    std::vector<std::vector<int8_t>> depth;
    Point start;
    Point end;
    size_t width;
    size_t height;

    Map(const Input& input)
          : width(input.front().size()),
            height(input.size()) {
        for (int32_t y = 0; y < height; y++) {
            std::vector<int8_t> row(width, 0);
            for (int32_t x = 0; x < width; x++) {
                switch (input.at(y).at(x)) {
                    case 'S': {
                        start = {x, y};
                        row.at(x) = 1;
                    } break;
                    case 'E': {
                        end = {x, y};
                        row.at(x) = static_cast<int8_t>('z' - 'a' + 1);
                    } break;
                    default: {
                        row.at(x) = static_cast<int8_t>(input.at(y).at(x) - 'a' + 1);
                    } break;
                }
            }
            depth.push_back(std::move(row));
        }
    }

    bool isInside(const Point& point) const {
        return point.x >= 0 && point.x < width && point.y >= 0 && point.y < height;
    }

    int8_t at(const Point& point) const {
        return depth.at(point.y).at(point.x);
    }
};

class Pathfinder {
    const std::set<Point> NEIGHBORHOOD{
        {0,  -1},
        {-1, 0 },
        {1,  0 },
        {0,  1 },
    };

  public:
    Pathfinder(const Input& input)
          : m_map(input),
            m_input(input) {}

  public:
    size_t findShortestPath() {  // Part one
        // A-star algorithm - starts at the start and searches the end
        std::map<Point, size_t> gscores;
        gscores[m_map.start] = 0;

        std::map<Point, size_t> fscores;
        fscores[m_map.start] = m_map.start.manhattan(m_map.end);

        m_path.clear();

        std::set<Point> queue{m_map.start};
        while (!queue.empty()) {
            Point current = bestCandidate(queue, fscores);

            if (current == m_map.end) {
                // End reached
                size_t steps{0};
                Point next = m_map.end;
                while (m_path.contains(next)) {
                    next = m_path.at(next);
                    steps++;
                }
                return steps;
            }

            for (const auto& step : NEIGHBORHOOD) {
                if (Point neighbor = current + step; m_map.isInside(neighbor)) {
                    if (m_map.at(neighbor) - m_map.at(current) < 2) {
                        size_t score = gscores.at(current) + 1;
                        if (!gscores.contains(neighbor) || score < gscores.at(neighbor)) {
                            gscores[neighbor] = score;
                            fscores[neighbor] = score + neighbor.manhattan(m_map.end);
                            m_path[neighbor] = current;
                            queue.insert(neighbor);
                        }
                    }
                }
            }
            queue.erase(current);
            // print(current);
        }
        return 0;
    }

    size_t findHikingTrail() {  // Part two
        // A-star algorithm - starts at the end and searches for the shortest way to 'a'
        static constexpr int8_t GOAL{1};

        std::map<Point, size_t> gscores;
        gscores[m_map.end] = 0;

        std::map<Point, size_t> fscores;
        fscores[m_map.end] = distanceToNearestGoal(m_map.end, GOAL);

        m_path.clear();
        indexGoals(GOAL);

        std::set<Point> queue{m_map.end};
        while (!queue.empty()) {
            Point current = *std::min_element(queue.begin(), queue.end(),
                                              [&fscores](const auto& a, const auto& b) {
                                                  return fscores.at(a) < fscores.at(b);
                                              });

            if (m_map.at(current) == GOAL) {
                // 'a' reached
                size_t steps{0};
                Point next = current;
                while (m_path.contains(next)) {
                    next = m_path.at(next);
                    steps++;
                }
                return steps;
            }

            for (const auto& step : NEIGHBORHOOD) {
                if (Point neighbor = current + step; m_map.isInside(neighbor)) {
                    if (m_map.at(current) - m_map.at(neighbor) < 2) {
                        size_t score = gscores.at(current) + 1;
                        if (!gscores.contains(neighbor) || score < gscores.at(neighbor)) {
                            gscores[neighbor] = score;
                            fscores[neighbor] = score + distanceToNearestGoal(neighbor, GOAL);
                            m_path[neighbor] = current;
                            queue.insert(neighbor);
                        }
                    }
                }
            }
            queue.erase(current);
            // print(current);
        }
        return 0;
    }

    size_t distanceToNearestGoal(const Point& pos, int8_t goal) const {
        size_t d_best = std::numeric_limits<size_t>::max();
        for (const auto& goal_pos : m_goals) {
            d_best = std::min(d_best, pos.manhattan(goal_pos));
        }
        return d_best;
    }

    std::set<Point> reconstruct(const Point& start) const {
        std::set<Point> path;
        Point current = start;
        while (m_path.contains(current)) {
            path.insert(current);
            current = m_path.at(current);
        }
        return path;
    }

    void print() const {
        print(m_map.end);
    }

  private:
    static Point bestCandidate(const std::set<Point>& queue,
                               const std::map<Point, size_t>& fscores) {
        return *std::min_element(queue.begin(), queue.end(),
                                 [&fscores](const auto& a, const auto& b) {
                                     return fscores.at(a) < fscores.at(b);
                                 });
    }

    void indexGoals(int8_t goal) {
        m_goals.clear();
        for (int32_t y = 0; y < m_map.height; y++) {
            for (int32_t x = 0; x < m_map.width; x++) {
                const Point goal_pos{x, y};
                if (m_map.at(goal_pos) == goal) {
                    for (const auto& step : NEIGHBORHOOD) {
                        if (Point neighbor = goal_pos + step;
                            m_map.isInside(neighbor) && goal - m_map.at(neighbor) < 2) {
                            // Index only achievable goals
                            m_goals.insert({x, y});
                        }
                    }
                }
            }
        }
    }

    void print(const Point& current) const {
        const auto path = reconstruct(current);
        std::string str{};
        for (int32_t y = 0; y < m_map.height; y++) {
            for (int32_t x = 0; x < m_map.width; x++) {
                if (path.contains({x, y})) {
                    str += console::green(std::string(1, m_input.at(y).at(x)));
                } else if (m_path.contains({x, y})) {
                    str += console::red(std::string(1, m_input.at(y).at(x)));
                } else {
                    str += m_input.at(y).at(x);
                }
            }
            str += '\n';
        }
        std::cout << str << std::endl;
    }

  private:
    const Input& m_input;
    const Map m_map;
    std::map<Point, Point> m_path;
    std::set<Point> m_goals;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        inputs.push_back(line);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Pathfinder finder(input);
    return finder.findShortestPath();
}

Result Day::partTwo(const Input& input) const {
    Pathfinder finder(input);
    return finder.findHikingTrail();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc