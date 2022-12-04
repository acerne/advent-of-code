#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

namespace aoc {

namespace {

bool isInside(uint8_t value, Section section) {
    return value >= section.first && value <= section.second;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        auto pair = helpers::tokenize(line, ',');
        auto sections_a = helpers::tokenize(pair.at(0), '-');
        auto sections_b = helpers::tokenize(pair.at(1), '-');
        Section a = {std::stoi(sections_a.at(0)), std::stoi(sections_a.at(1))};
        Section b = {std::stoi(sections_b.at(0)), std::stoi(sections_b.at(1))};
        inputs.push_back({a, b});
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Result count = 0;
    for (const auto& [a, b] : input) {
        if (b.first >= a.first && b.second <= a.second
            || a.first >= b.first && a.second <= b.second) {
            count++;
        }
    }
    return count;
}

Result Day::partTwo(const Input& input) const {
    Result count = 0;
    for (const auto& [a, b] : input) {
        if (isInside(a.first, b) || isInside(a.second, b) || isInside(b.first, a)
            || isInside(b.second, a)) {
            count++;
        }
    }
    return count;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc