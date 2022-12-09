#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <cmath>
#include <set>

namespace aoc {

struct Point {
    int32_t x{0};
    int32_t y{0};

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
};

class RopeBridge {
  public:
    RopeBridge(size_t n_knots)
          : m_knots(n_knots, {0, 0}) {}

  public:
    void step(char direction, int32_t steps) {
        for (int32_t n = 0; n < steps; n++) {
            auto previous = m_knots.front();
            m_knots.front() = move(m_knots.front(), direction);
            for (size_t i = 1; i < m_knots.size(); i++) {
                auto diff = m_knots.at(i - 1) - m_knots.at(i);
                if (abs(diff.x) > 1 || abs(diff.y) > 1) {
                    m_knots.at(i).x += helpers::sign(diff.x);
                    m_knots.at(i).y += helpers::sign(diff.y);
                }
            }
            m_visited.insert(m_knots.back());
        }
    }

    size_t placesVisited() const {
        return m_visited.size();
    }

  private:
    static Point move(Point head, char direction) {
        switch (direction) {
            case 'L': {
                head.x--;
            } break;
            case 'R': {
                head.x++;
            } break;
            case 'U': {
                head.y++;
            } break;
            case 'D': {
                head.y--;
            } break;
            default:
                throw std::runtime_error("Invalid state");
        }
        return head;
    }

  private:
    std::vector<Point> m_knots;
    std::set<Point> m_visited;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        auto [direction, step] = helpers::split(line, ' ');
        inputs.push_back({direction.front(), std::stoi(step)});
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    RopeBridge bridge(2);
    for (const auto& instruction : input) {
        bridge.step(instruction.first, instruction.second);
    }
    return bridge.placesVisited();
}

Result Day::partTwo(const Input& input) const {
    RopeBridge bridge(10);
    for (const auto& instruction : input) {
        bridge.step(instruction.first, instruction.second);
    }
    return bridge.placesVisited();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc