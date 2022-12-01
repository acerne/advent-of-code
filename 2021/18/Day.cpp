#include "Day.h"

#include "FactoryManager.h"

#include <iostream>
#include <list>
#include <optional>

namespace aoc {

class Snailfish {
  public:  // Methods
    Snailfish(const std::string& input) {
        recursive_parser(input, 0);
    }

    void add(Snailfish other) {
        m_snailfish.splice(m_snailfish.end(), other.m_snailfish, other.m_snailfish.begin(),
                           other.m_snailfish.end());
        for (auto& node : m_snailfish) {
            node.depth++;
        }
        bool reduced = false;
        while (!reduced) {
            // draw();
            bool did_explode = explode();
            if (!did_explode) {
                bool did_split = split();
                reduced = !did_explode && !did_split;
            }
        }
    }

    void draw() const {
        for (const auto& node : m_snailfish) {
            if (node.depth >= 4) {
                std::cout << "\033[31m";
            } else if (node.value > 9) {
                std::cout << "\033[33m";
            } else {
                std::cout << "\033[32m";
            }
            if (node.pos == Node::Pos::LEFT) {
                std::cout << std::string(node.depth, ' ') << '/' << node.value << std::endl;
            } else {
                std::cout << std::string(node.depth, ' ') << '\\' << node.value << std::endl;
            }
        }
        std::cout << "\033[39m";
        std::cout << "-----" << std::endl;
    }

    size_t magnitude() {
        auto flat = m_snailfish;
        for (int depth = 3; depth > 0; depth--) {
            for (auto it = flat.begin(), prev = flat.end(); it != flat.end();) {
                if (it->depth == depth) {
                    if (it->pos == Node::Pos::RIGHT) {
                        int value = 3 * prev->value + 2 * it->value;
                        it = flat.erase(it);
                        prev = flat.erase(prev);
                        prev--;
                        if (prev->depth == depth - 1 && prev->pos == Node::Pos::LEFT) {
                            flat.insert(it, Node{value, depth - 1, Node::Pos::RIGHT});
                        } else if (it->depth == depth - 1 && it->pos == Node::Pos::RIGHT) {
                            flat.insert(it, Node{value, depth - 1, Node::Pos::LEFT});
                        } else {
                            flat.insert(it, Node{value, depth - 1, Node::Pos::LEFT});
                        }
                    }
                }
                prev = it;
                it++;
            }
        }
        size_t mag = 3 * flat.front().value + 2 * flat.back().value;
        return mag;
    }

  protected:  // Methods
    bool explode() {
        for (auto it = m_snailfish.begin(), prev = m_snailfish.end();
             it != m_snailfish.end();) {
            if (it->depth > 4) {
                throw std::runtime_error("This should not happen!");
            }
            if (it->depth == 4) {
                Node insert{0, 3, Node::Pos::RIGHT};
                // Explode left
                if (prev != m_snailfish.end()) {  // not first node
                    prev->value += it->value;
                    if (prev->pos == Node::Pos::RIGHT && prev->depth == 3 || prev->depth < 3) {
                        insert = Node{0, 3, Node::Pos::LEFT};
                    }
                } else {
                    insert = Node{0, 3, Node::Pos::LEFT};
                }
                it = m_snailfish.erase(it);
                prev = it;
                ++it;
                // Explode right
                if (it != m_snailfish.end()) {  // not last node
                    it->value += prev->value;
                }
                m_snailfish.erase(prev);
                m_snailfish.insert(it, insert);

                return true;  // explode only left-most pair
            } else {
                prev = it;
                ++it;
            }
        }
        return false;
    }

    bool split() {
        for (auto it = m_snailfish.begin(); it != m_snailfish.end(); ++it) {
            if (it->value > 9) {
                auto a = it->value / 2;
                auto b = it->value - a;
                auto new_depth = it->depth + 1;
                *it = Node{b, new_depth, Node::Pos::RIGHT};
                m_snailfish.insert(it, Node{a, new_depth, Node::Pos::LEFT});
                return true;
            }
        }
        return false;
    }

  private:  // Methods
    static int find_separator(const std::string& input) {
        int separator;
        int bracket_counter = 0;
        for (int i = 0; i < input.size(); i++) {
            switch (input.at(i)) {
                case '[':
                    bracket_counter++;
                    break;
                case ']':
                    bracket_counter--;
                    break;
                case ',':
                    if (bracket_counter == 1) {
                        separator = i;
                    };
                    break;
                default:
                    break;
            }
        }
        return separator;
    }

    std::optional<int> recursive_parser(const std::string& input, int depth) {
        if (input.size() == 1) {
            return std::stoi(input);
        }
        int separator = find_separator(input);
        const auto left = input.substr(1, separator - 1);
        const auto right = input.substr(separator + 1, input.size() - separator - 2);

        auto left_value = recursive_parser(left, depth + 1);
        if (left_value) {
            m_snailfish.push_back(
                Node{.value = *left_value, .depth = depth, .pos = Node::Pos::LEFT});
        }
        auto right_value = recursive_parser(right, depth + 1);
        if (right_value) {
            m_snailfish.push_back(
                Node{.value = *right_value, .depth = depth, .pos = Node::Pos::RIGHT});
        }
        return {};
    }

  private:  // Classes
    struct Node {
        int value;
        int depth;
        enum class Pos {
            LEFT,
            RIGHT
        } pos;
    };

  private:  // Members
    std::list<Node> m_snailfish;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        inputs.emplace_back(line);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    auto sum = input.front();
    for (int i = 1; i < input.size(); i++) {
        // sum.draw();
        sum.add(input.at(i));
    }
    // sum.draw();
    auto mag = sum.magnitude();
    return mag;
}

Result Day::partTwo(const Input& input) const {
    size_t max = 0;
    for (const auto& b : input) {
        for (auto a : input) {
            a.add(b);
            max = std::max(max, a.magnitude());
        }
    }
    return max;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc