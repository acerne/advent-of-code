#include "Day00.h"

#include "FactoryManager.h"

#include <fstream>
#include <numeric>

namespace aoc {

namespace {

Input read(const std::string& file_path) {
    Input inputs;
    std::ifstream line_stream(file_path);
    if (!line_stream.good()) {
        throw std::runtime_error("File does not exist");
    }
    std::string line;
    while (std::getline(line_stream, line)) {
        inputs.push_back(std::stoi(line));
    }
    return inputs;
}

size_t count_increments(const Input& values) {
    size_t increments = 0;
    for (size_t i = 1; i < values.size(); i++) {
        if (values.at(i) > values.at(i - 1)) {
            increments++;
        }
    }
    return increments;
}

std::vector<int32_t> window_sum(const Input& inputs, size_t window_size) {
    std::vector<int32_t> sums;
    int32_t rolling_sum = std::accumulate(inputs.begin(), inputs.begin() + window_size, 0);
    sums.push_back(rolling_sum);
    for (size_t i = window_size; i < inputs.size(); i++) {
        rolling_sum -= inputs.at(i - window_size);
        rolling_sum += inputs.at(i);
        sums.push_back(rolling_sum);
    }
    return sums;
}

}  // namespace

void Day00::loadSample() {
    m_input = read(FOLDER + "data/sample.txt");
}

void Day00::loadInput() {
    m_input = read(FOLDER + "data/input.txt");
}

void Day00::solvePartOne() {
    m_result = count_increments(m_input);
}

bool Day00::checkPartOne() const {
    return m_result == RESULT_PART_ONE;
}

void Day00::solvePartTwo() {
    const auto sums = window_sum(m_input, 3);
    m_result = count_increments(sums);
}

std::string Day00::expectedPartOne() const {
    return std::to_string(RESULT_PART_ONE);
}

bool Day00::checkPartTwo() const {
    return m_result == RESULT_PART_TWO;
}

std::string Day00::expectedPartTwo() const {
    return std::to_string(RESULT_PART_TWO);
}

std::string Day00::result() const {
    return std::to_string(m_result);
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day00>("Day00");
}

}  // namespace aoc