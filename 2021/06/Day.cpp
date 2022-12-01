#include "Day.h"

#include "FactoryManager.h"

#include <deque>
#include <fstream>
#include <numeric>
#include <sstream>

namespace aoc {

namespace {

size_t simulate(const Input& input, size_t n_days) {
    std::deque<size_t> stack(9, 0);
    for (const auto& fish : input) {
        stack.at(fish)++;
    }
    for (size_t day = 0; day < n_days; day++) {
        auto spawning = stack.front();
        stack.pop_front();
        stack.push_back(spawning);
        stack.at(6) += spawning;
    }
    size_t count = std::accumulate(stack.begin(), stack.end(), size_t{0});
    return count;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line, number;
    std::getline(line_stream, line);
    std::stringstream ss(line);
    while (std::getline(ss, number, ',')) {
        inputs.push_back(std::stoi(number));
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    return simulate(input, 80);
}

Result Day::partTwo(const Input& input) const {
    return simulate(input, 256);
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc