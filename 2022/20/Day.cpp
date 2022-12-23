#include "Day.h"

#include "FactoryManager.h"
#include "console.h"
#include "helpers.h"

#include <iostream>

namespace aoc {

struct Node {
    int64_t value;
    Node* prev{nullptr};
    Node* next{nullptr};
};

inline int64_t mod(int64_t a, int64_t b) {
    return (a % b + b) % b;
}

class CyclicDecoder {
  public:
    CyclicDecoder(const Input& input, int64_t key = 1) {
        // Using circular linked list
        for (int i = 0; i < input.size(); i++) {
            m_list[i] = {.value = input.at(i) * key};
        }
        m_list[0].prev = &m_list[input.size() - 1];
        m_list[0].next = &m_list[1];
        for (int i = 1; i < input.size() - 1; i++) {
            m_list[i].prev = &m_list[i - 1];
            m_list[i].next = &m_list[i + 1];
        }
        m_list[input.size() - 1].prev = &m_list[input.size() - 2];
        m_list[input.size() - 1].next = &m_list[0];
    }

  public:
    void move(int index) {
        // std::cout << "Moving " << m_values.at(index) << std::endl;
        Node* current = &m_list[index];
        if (current->value != 0) {
            Node* prev = current->prev;
            Node* next = current->next;

            // Remove from old spot and update neighbors
            prev->next = next;
            next->prev = prev;

            // Find new spot
            int64_t steps_forward;
            if (current->value > 0) {
                steps_forward = mod(current->value, m_list.size() - 1);
            } else {
                steps_forward = mod(m_list.size() - 1 + current->value, m_list.size() - 1);
            }
            int64_t steps_back = m_list.size() - 1 - steps_forward;
            if (steps_back < steps_forward) {
                // Scan backwards
                for (int64_t i = 0; i < steps_back; i++) {
                    prev = prev->prev;
                }
                next = prev->next;
            } else {
                // Scan forward
                for (int64_t i = 0; i < steps_forward; i++) {
                    next = next->next;
                }
                prev = next->prev;
            }

            // Insert bewteween new neighbors;
            current->prev = prev;
            current->next = next;
            next->prev = current;
            prev->next = current;
        }
    }

    bool validate() const {
        for (const auto& [value, node] : m_list) {
            if (!node.next || node.next->prev != &node) {
                return false;
            }
            if (!node.prev || node.prev->next != &node) {
                return false;
            }
        }
        return true;
    }

    int64_t score() const {
        int64_t score{0};
        auto zero = std::find_if(m_list.begin(), m_list.end(),
                                 [](auto node) { return node.second.value == 0; });
        Node* next = zero->second.next;
        for (int n = 1; n <= 3; n++) {
            for (int i = 0; i < 1000; i++) {
                next = next->next;
            }
            score += next->prev->value;
        }
        return score;
    }

  private:
    std::map<int, Node> m_list;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        inputs.push_back(std::stoi(line));
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    CyclicDecoder decoder(input);
    if (!decoder.validate()) {
        std::cout << "List corruption!" << std::endl;
    }
    for (int i = 0; i < input.size(); i++) {
        decoder.move(i);
    }
    if (!decoder.validate()) {
        std::cout << "List corruption!" << std::endl;
    }
    return decoder.score();
}

Result Day::partTwo(const Input& input) const {
    CyclicDecoder decoder(input, 811589153);
    if (!decoder.validate()) {
        std::cout << "List corruption!" << std::endl;
    }
    for (int n = 0; n < 10; n++) {
        for (int i = 0; i < input.size(); i++) {
            decoder.move(i);
        }
    }
    if (!decoder.validate()) {
        std::cout << "List corruption!" << std::endl;
    }
    return decoder.score();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc