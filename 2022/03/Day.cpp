#include "Day.h"

#include "FactoryManager.h"

namespace aoc {

namespace {

Result priority(char item) {
    if (item > 'a') {
        return item - 'a' + 1;
    } else {
        return item - 'A' + 27;
    }
}

Result duplicates(const std::string& first, const std::string& second) {
    for (const auto& c : first) {
        if (second.find(c) != std::string::npos) {
            return priority(c);
        }
    }
    return 0;
}

Result badges(const std::string& first, const std::string& second, const std::string& third) {
    for (const auto& c : first) {
        if (second.find(c) != std::string::npos && third.find(c) != std::string::npos) {
            return priority(c);
        }
    }
    return 0;
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
    Result sum = 0;
    for (const auto& line : input) {
        size_t half = line.size() / 2;
        sum += duplicates(line.substr(0, half), line.substr(half, half));
    }
    return sum;
}

Result Day::partTwo(const Input& input) const {
    Result sum = 0;
    for (int i = 0; i < input.size(); i += 3) {
        sum += badges(input.at(i), input.at(i + 1), input.at(i + 2));
    }
    return sum;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc