#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>

namespace aoc {

namespace {

bool match(char a, char b) {
    switch (a) {
        case '(':
            if (b == ')') {
                return true;
            }
            break;
        case '[':
            if (b == ']') {
                return true;
            }
            break;
        case '{':
            if (b == '}') {
                return true;
            }
            break;
        case '<':
            if (b == '>') {
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

size_t evaluate(char a) {
    switch (a) {
        case ')':
            return 3;
        case ']':
            return 57;
        case '}':
            return 1197;
        case '>':
            return 25137;
        default:
            return 0;
    }
}

void remove_matching(std::string& line) {
    bool done = false;
    while (!done) {
        done = true;
        for (int i = 0; i < line.size() - 1; i++) {
            if (match(line.at(i), line.at(i + 1))) {
                auto it = line.erase(line.begin() + i);
                line.erase(it);
                done = false;
            }
        }
    }
}

size_t count_corrupted(const Input& input) {
    size_t sum = 0;
    for (const auto& line : input) {
        auto leftover = line;
        remove_matching(leftover);
        for (const auto& c : leftover) {
            const auto weight = evaluate(c);
            if (weight > 0) {
                sum += weight;
                break;
            }
        }
    }
    return sum;
}

void remove_corrupted(Input& input) {
    for (auto line_it = input.begin(); line_it != input.end();) {
        auto leftover = *line_it;
        remove_matching(leftover);
        size_t sum = 0;
        for (const auto& c : leftover) {
            sum += evaluate(c);
        }
        if (sum > 0) {
            line_it = input.erase(line_it);
        } else {
            ++line_it;
        }
    }
}

size_t count_incomplete(const Input& input) {
    std::vector<size_t> sums;
    for (const auto& line : input) {
        auto leftover = line;
        remove_matching(leftover);

        size_t sum = 0;
        for (auto rev_it = leftover.rbegin(); rev_it != leftover.rend(); ++rev_it) {
            sum *= 5;
            switch (*rev_it) {
                case '(':
                    sum += 1;
                    break;
                case '[':
                    sum += 2;
                    break;
                case '{':
                    sum += 3;
                    break;
                case '<':
                    sum += 4;
                    break;
                default:
                    break;
            }
        }
        sums.push_back(sum);
    }
    std::sort(sums.begin(), sums.end());
    return sums.at((sums.size() - 1) / 2);
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        inputs.push_back(line);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    const auto sum = count_corrupted(input);
    return sum;
}

Result Day::partTwo(const Input& input) const {
    auto lines = input;
    remove_corrupted(lines);
    const auto sum = count_incomplete(lines);
    return sum;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc