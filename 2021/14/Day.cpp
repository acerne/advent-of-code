#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>
#include <map>
#include <unordered_set>

namespace aoc {

namespace {

struct Node {
    char a;
    char b;
    uint8_t depth;

    bool operator==(const Node& other) const {
        return a == other.a && b == other.b && depth == other.depth;
    }

    bool operator<(const Node& other) const {
        return depth < other.depth || (depth == other.depth && a < other.a)
               || (depth == other.depth && a == other.a && b < other.b);
    }
};

class PolymerCounter {
  public:
    PolymerCounter(Rules insertion_rules)
          : m_insertion_rules(std::move(insertion_rules)) {}

    void run(std::string polymer_template, uint8_t n_insertions) {
        m_char_counters.clear();
        for (int i = 0; i < polymer_template.size() - 1; i++) {
            m_char_counters[polymer_template.at(i)]++;
            const auto counts = search_node(Node{.a = polymer_template.at(i),
                                                 .b = polymer_template.at(i + 1),
                                                 .depth = n_insertions});
            for (const auto& [key, value] : counts) {
                m_char_counters[key] += value;
            }
        }
        m_char_counters[polymer_template.back()]++;
    }

    size_t most_common_count() const {
        const auto comparison = [](auto& l, auto& r) -> bool {
            return l.second < r.second;
        };
        const auto max =
            std::max_element(m_char_counters.begin(), m_char_counters.end(), comparison);
        return max->second;
    }

    size_t least_common_count() const {
        const auto comparison = [](auto& l, auto& r) -> bool {
            return l.second < r.second;
        };
        const auto min =
            std::min_element(m_char_counters.begin(), m_char_counters.end(), comparison);
        return min->second;
    }

  private:
    std::map<char, size_t> search_node(const Node& idx) {
        uint8_t next = idx.depth - 1;
        if (m_node_cache.count(idx) > 0) {  // node exists, return cached counts
            return m_node_cache.at(idx);
        } else if (next > 0) {  // generate node
            std::map<char, size_t> counters;
            char x = m_insertion_rules.at({idx.a, idx.b});
            counters[x]++;

            Node left{.a = idx.a, .b = x, .depth = next};
            for (const auto& [key, value] : search_node(left)) {
                counters[key] += value;
            }

            Node right{.a = x, .b = idx.b, .depth = next};
            for (const auto& [key, value] : search_node(right)) {
                counters[key] += value;
            }

            m_node_cache.emplace(idx, counters);
            return counters;
        } else {  // bottomed out
            char x = m_insertion_rules.at({idx.a, idx.b});
            return std::map<char, size_t>{
                {x, 1}
            };
        }
    }

  private:
    const Rules m_insertion_rules;
    std::map<Node, std::map<char, size_t>> m_node_cache;
    std::map<char, size_t> m_char_counters;
};

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    std::getline(line_stream, line);
    inputs.polymer_template = line;
    std::getline(line_stream, line);
    while (std::getline(line_stream, line)) {
        inputs.insertion_rules.emplace(std::pair<char, char>{line.at(0), line.at(1)},
                                       line.at(6));
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    PolymerCounter counter(input.insertion_rules);
    counter.run(input.polymer_template, 10);
    const auto min = counter.least_common_count();
    const auto max = counter.most_common_count();
    return max - min;
}

Result Day::partTwo(const Input& input) const {
    PolymerCounter counter(input.insertion_rules);
    counter.run(input.polymer_template, 40);
    const auto min = counter.least_common_count();
    const auto max = counter.most_common_count();
    return max - min;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc