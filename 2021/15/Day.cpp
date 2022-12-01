#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>
#include <iostream>
#include <set>

namespace aoc {

namespace {

class Dijkstra {
  public:
    Dijkstra(const Input& input)
          : m_risk(input.risk),
            m_start(input.start),
            m_stop(input.stop) {}

    size_t run() {
        m_track.clear();
        m_candidates.clear();
        m_score.clear();

        m_candidates.insert(m_start);

        for (const auto& [coord, _] : m_risk) {
            m_score[coord] = std::numeric_limits<size_t>::max();
        }
        m_score[m_start] = 0;

        while (!m_candidates.empty()) {
            const auto current = take_best(m_candidates);
            if (current == m_stop) {
                break;
            }

            m_candidates.erase(current);

            auto neighbors = get_neighbors(current);
            for (const auto& neighbor : neighbors) {
                auto score = m_score.at(current) + m_risk.at(neighbor);
                if (score < m_score.at(neighbor)) {
                    m_track[neighbor] = current;
                    m_score.at(neighbor) = score;
                    m_candidates.insert(neighbor);
                }
            }
        };

        return m_score.at(m_stop);
    }

    void draw() {
        draw(m_stop);
    }

  private:
    Coord take_best(const std::set<Coord>& candidates) {
        Coord best;
        auto lowest_score = std::numeric_limits<size_t>::max();
        for (const auto& candidate : candidates) {
            const auto score = m_score.at(candidate);
            if (score < lowest_score) {
                lowest_score = score;
                best = candidate;
            }
        }
        return best;
    }

    std::vector<Coord> get_neighbors(const Coord& current) {
        std::vector<Coord> neighbors;
        if (current.x < m_stop.x) {
            neighbors.emplace_back(Coord{current.x + 1, current.y});
        }
        if (current.y < m_stop.y) {
            neighbors.emplace_back(Coord{current.x, current.y + 1});
        }
        if (current.x > m_start.x) {
            neighbors.emplace_back(Coord{current.x - 1, current.y});
        }
        if (current.y > m_start.y) {
            neighbors.push_back(Coord{current.x, current.y - 1});
        }
        return neighbors;
    }

    void draw(const Coord& current) {
        std::vector<Coord> path;
        path.push_back(current);
        while (path.back() != m_start) {
            path.push_back(m_track.at(path.back()));
        }
        size_t risk_sum = 0;
        for (int y = m_start.y; y <= m_stop.y; y++) {
            for (int x = m_start.x; x <= m_stop.x; x++) {
                Coord coord{x, y};
                if (std::find(path.begin(), path.end(), coord) != path.end()) {
                    std::cout << "\033[32m";
                    risk_sum += m_risk.at(coord);
                } else {
                    std::cout << "\033[39m";
                }
                std::cout << int(m_risk.at(coord)) << ' ';
            }
            std::cout << std::endl;
        }
        risk_sum -= m_risk.at(m_start);
        std::cout << std::endl << "Risk sum: " << risk_sum << std::endl;
    }

  private:
    const std::map<Coord, int8_t> m_risk;
    const Coord m_start;
    const Coord m_stop;
    std::set<Coord> m_candidates;
    std::map<Coord, Coord> m_track;
    std::map<Coord, size_t> m_score;
};

Input expand_map(const Input& input, int n) {
    Input expanded;
    Coord size{input.stop.x + 1, input.stop.y + 1};
    expanded.start = input.start;
    expanded.stop = Coord{size.x * n - 1, size.y * n - 1};
    for (const auto& [coord, risk] : input.risk) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                Coord new_coord{i * size.x + coord.x, j * size.y + coord.y};
                expanded.risk[new_coord] = (risk + i + j - 1) % 9 + 1;
            }
        }
    }
    return expanded;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    Coord coord{0, 0};
    inputs.start = coord;
    while (std::getline(line_stream, line)) {
        coord.x = 0;
        for (const auto& c : line) {
            inputs.risk[coord] = c - '0';
            coord.x++;
        }
        coord.y++;
    }
    inputs.stop = Coord{coord.x - 1, coord.y - 1};
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Dijkstra solver(input);
    const auto risk = solver.run();
    // solver.draw();
    return risk;
}

Result Day::partTwo(const Input& input) const {
    const auto expanded = expand_map(input, 5);
    Dijkstra solver(expanded);
    const auto risk = solver.run();
    // solver.draw();
    return risk;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc