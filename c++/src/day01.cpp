#include "day01.hpp"

#include <fstream>
#include <numeric>

namespace
{
size_t count_increments(const std::vector<int32_t>& values) {
  size_t increments = 0;
  for (size_t i = 1; i < values.size(); i++) {
    if (values.at(i) > values.at(i - 1)) {
      increments++;
    }
  }
  return increments;
}

std::vector<int32_t> window_sum(const std::vector<int32_t>& inputs, size_t window_size) {
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
} // namespace

Day01::Day01()
  : Day(1)
  , m_path("../data/day01.txt") {
  m_parts.push_back(std::make_unique<Part1>(*this));
  m_parts.push_back(std::make_unique<Part2>(*this));
}

void Day01::load() {
  std::ifstream line_stream(m_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    m_inputs.push_back(std::stoi(line));
  }
}

Day01::Part1::Part1(const Day01& day)
  : Part(1)
  , m_day(day) {}

void Day01::Part1::process() {
  m_result = count_increments(m_day.m_inputs);
}

Day01::Part2::Part2(const Day01& day)
  : Part(2)
  , m_day(day) {}

void Day01::Part2::process() {
  const auto sums = window_sum(m_day.m_inputs, 3);
  m_result = count_increments(sums);
}
