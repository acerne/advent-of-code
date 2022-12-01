#include "Day.h"

#include "FactoryManager.h"

#include <numeric>

namespace aoc {

namespace {

std::vector<int> sum_calories(const Input& values) {
    std::vector<int> sums;
    for (const auto& elf : values) {
        sums.push_back(std::accumulate(elf.begin(), elf.end(), 0));
    }
    return sums;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    std::vector<int32_t> elf;
    while (std::getline(line_stream, line)) {
        if (line.empty()) {
            inputs.push_back(elf);
            elf.clear();
        } else {
            elf.push_back(std::stoi(line));
        }
    }
    inputs.push_back(elf);
    return inputs;
}

Result Day::partOne(const Input& input) const {
    auto sums = sum_calories(input);
    return *std::max_element(sums.begin(), sums.end());
}

Result Day::partTwo(const Input& input) const {
    auto sums = sum_calories(input);
    std::sort(sums.begin(), sums.end());
    int32_t top_three = 0;
    for (auto it = sums.rbegin(); it != sums.rbegin() + 3; ++it) {
        top_three += *it;
    }
    return top_three;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc