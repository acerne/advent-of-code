#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>
#include <set>
#include <variant>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace aoc {

class Packet {
    enum class Cmp {
        Lesser,
        Equals,
        Greater,
    };

  public:
    Packet(const std::string& str)
          : m_str(str) {
        size_t start = str.find_first_of('[');
        size_t end = str.find_last_of(']');
        size_t idx = start + 1;
        std::string num{};
        while (idx < end) {
            switch (str.at(idx)) {
                case '[': {
                    size_t idx_closing = closingBracket(str, idx);
                    m_contents.push_back(Packet(str.substr(idx, idx_closing - idx + 1)));
                    idx = idx_closing;
                } break;
                case '0' ... '9': {
                    size_t idx_closing =
                        std::min(str.find_first_of(',', idx), str.find_first_of(']', idx));
                    m_contents.push_back(std::stoi(str.substr(idx, idx_closing - idx)));
                    idx = idx_closing;
                } break;
                default:
                    idx++;
                    break;
            }
        }
    }

    Packet(int value)
          : m_str(std::to_string(value)) {
        m_contents.push_back(value);
    }

    bool operator<=(const Packet& other) const {  // Part 1
        Cmp order = compare(Packet(*this), other);
        return order != Cmp::Greater;
    }

    bool operator<(const Packet& other) const {  // Part 2 (used by std::set<Packet>)
        Cmp order = compare(Packet(*this), other);
        return order == Cmp::Lesser;
    }

  private:
    static Cmp compare(const Packet& first, const Packet& second) {
        for (size_t i = 0; i < std::min(first.m_contents.size(), second.m_contents.size());
             i++) {
            Cmp order =
                std::visit(overloaded{
                               [](int a, int b) { return compare(a, b); },
                               [](int a, const Packet& b) { return compare(Packet(a), b); },
                               [](const Packet& a, int b) { return compare(a, Packet(b)); },
                               [](const Packet& a, const Packet& b) { return compare(a, b); },
                           },
                           first.m_contents.at(i), second.m_contents.at(i));
            if (order != Cmp::Equals) {
                return order;
            }
        }
        return compare(first.m_contents.size(), second.m_contents.size());
    }

    static Cmp compare(int a, int b) {
        if (a > b) {
            return Cmp::Greater;
        } else if (a < b) {
            return Cmp::Lesser;
        }
        return Cmp::Equals;
    }

    static size_t closingBracket(const std::string& input, size_t opening_bracket) {
        int bracket_counter = 0;
        for (int i = opening_bracket; i < input.size(); i++) {
            switch (input.at(i)) {
                case '[': {
                    bracket_counter++;
                } break;
                case ']': {
                    bracket_counter--;
                    if (bracket_counter == 0) {
                        return i;
                    }
                } break;
                default:
                    break;
            }
        }
        return std::string::npos;
    }

  private:
    const std::string m_str;
    std::vector<std::variant<Packet, int>> m_contents;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        if (!line.empty()) {
            Packet a(line);
            std::getline(line_stream, line);
            Packet b(line);
            inputs.push_back({a, b});
        }
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    size_t correct_order{0};
    size_t i{1};
    for (const auto& [first, second] : input) {
        if (first <= second) {
            correct_order += i;
        }
        i++;
    }
    return correct_order;
}

Result Day::partTwo(const Input& input) const {
    std::set<Packet> ordered;
    for (const auto& [first, second] : input) {
        ordered.emplace(first);
        ordered.emplace(second);
    }

    Packet divider2("[[2]]");
    Packet divider6("[[6]]");

    ordered.insert(divider2);
    ordered.insert(divider6);

    size_t idx_divider2 = std::distance(ordered.begin(), ordered.find(divider2)) + 1;
    size_t idx_divider6 = std::distance(ordered.begin(), ordered.find(divider6)) + 1;

    return idx_divider2 * idx_divider6;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc