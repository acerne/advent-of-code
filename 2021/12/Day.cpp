#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>
#include <map>
#include <set>

namespace aoc {

namespace {

std::multimap<std::string, std::string> map_cave(const Input& input) {
    std::multimap<std::string, std::string> cave_map;
    for (const auto& tunnel : input) {
        if (tunnel.first != "end") {
            cave_map.insert(tunnel);
        }
        if (tunnel.second != "end") {
            cave_map.insert({tunnel.second, tunnel.first});
        }
    }
    return cave_map;
}

class Explorer {
  public:
    Explorer(const std::multimap<std::string, std::string>& cave_map,
             bool time_to_visit_one_small_cave_twice = false)
          : m_cave_map(cave_map),
            m_path_count(0),
            m_visit_twice(time_to_visit_one_small_cave_twice) {}

    size_t search() {
        recursive_search("start", {}, false);
        return m_path_count;
    }

  private:
    void recursive_search(const std::string& current, std::vector<std::string> trace,
                          bool second_visit_spent) {
        if (current == "start" && trace.size() > 0) {
            return;
        }
        if (is_small(current)) {
            if (std::find(trace.begin(), trace.end(), current) != trace.end()) {
                if (m_visit_twice) {
                    if (second_visit_spent) {
                        return;
                    } else {
                        second_visit_spent = true;
                    }
                } else {
                    return;
                }
            }
        }
        trace.push_back(current);

        if (current == "end") {
            m_path_count++;
            m_paths.push_back(trace);
            return;
        }

        auto range = m_cave_map.equal_range(current);
        for (auto it = range.first; it != range.second; ++it) {
            const auto next = it->second;
            recursive_search(next, trace, second_visit_spent);
        }
    }

    bool is_small(const std::string& cave) {
        return cave != "start" && cave != "end" && cave.front() >= 'a' && cave.front() <= 'z';
    }

  private:
    const std::multimap<std::string, std::string>& m_cave_map;
    std::vector<std::vector<std::string>> m_paths;
    size_t m_path_count;
    const bool m_visit_twice;
};

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        std::string first, second;
        std::stringstream ss(line);
        std::getline(ss, first, '-');
        std::getline(ss, second, '-');
        inputs.emplace_back(first, second);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    const auto map = map_cave(input);
    Explorer explorer(map);
    const auto count = explorer.search();
    return count;
}

Result Day::partTwo(const Input& input) const {
    const auto map = map_cave(input);
    Explorer explorer(map, true);
    const auto count = explorer.search();
    return count;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc