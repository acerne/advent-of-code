#include "Day01.h"

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

std::vector<int> sum_calories(const Input& values) {
    std::vector<int> sums;
    for (const auto& elf : values) {
        sums.push_back(std::accumulate(elf.begin(), elf.end(), 0));
    }
    return sums;
}

}  // namespace

void Day01::loadSample() {
    m_input = read(FOLDER + "data/sample.txt");
}

void Day01::loadInput() {
    m_input = read(FOLDER + "data/input.txt");
}

void Day01::solvePartOne() {
    auto sums = sum_calories(m_input);
    m_result = *std::max_element(sums.begin(), sums.end());
}

bool Day01::checkPartOne() const {
    return m_result == RESULT_PART_ONE;
}

void Day01::solvePartTwo() {
    auto sums = sum_calories(m_input);
    std::sort(sums.begin(), sums.end());
    int32_t top_three = 0;
    for (auto it = sums.rbegin(); it != sums.rbegin() + 3; ++it) {
        top_three += *it;
    }
    m_result = top_three;
}

std::string Day01::expectedPartOne() const {
    return std::to_string(RESULT_PART_ONE);
}

bool Day01::checkPartTwo() const {
    return m_result == RESULT_PART_TWO;
}

std::string Day01::expectedPartTwo() const {
    return std::to_string(RESULT_PART_TWO);
}

std::string Day01::result() const {
    return std::to_string(m_result);
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day01>("Day01");
}

}  // namespace aoc