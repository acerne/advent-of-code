

#include "datatypes.hpp"
#include "day.hpp"

#include <fstream>
#include <numeric>

namespace
{
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
} // namespace

void Day::load() {
  std::ifstream line_stream("../" + Day::to_string_id(DAY_) + "/data.txt");
  std::string line;
  while (std::getline(line_stream, line)) {
    m_input.push_back(std::stoi(line));
  }
}

void Day::Part1::process() {
  m_result = count_increments(m_input);
}

void Day::Part2::process() {
  const auto sums = window_sum(m_input, 3);
  m_result = count_increments(sums);
}

extern "C" Day* create_object(int num) {
  return new Day();
}

extern "C" void destroy_object(Day* object) {
  delete object;
}