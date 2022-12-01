#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>
#include <sstream>

namespace aoc {

namespace {

//

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
    size_t min_cost = std::numeric_limits<size_t>::max();
    int16_t min_pos;
    int16_t n_pos = *std::max_element(input.begin(), input.end());
    for (int16_t pos = 0; pos <= n_pos; pos++) {
        size_t cost = 0;
        for (const auto& crab : input) {
            cost += std::abs(crab - pos);
        }
        if (cost < min_cost) {
            min_pos = pos;
            min_cost = cost;
        }
    }
    return min_cost;
}

Result Day::partTwo(const Input& input) const {
    size_t min_cost = std::numeric_limits<size_t>::max();
    int16_t min_pos;
    int16_t n_pos = *std::max_element(input.begin(), input.end());
    for (int16_t pos = 0; pos <= n_pos; pos++) {
        size_t cost = 0;
        for (const auto& crab : input) {
            int16_t diff = std::abs(crab - pos);
            cost += (diff * (diff + 1)) / 2;
        }
        if (cost < min_cost) {
            min_pos = pos;
            min_cost = cost;
        }
    }
    return min_cost;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc