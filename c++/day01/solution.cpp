

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

Input Day::read(const std::string& file_path) {
  Input data;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    data.push_back(std::stoi(line));
  }
  return data;
}

Result Day::Part1::process(const Input& input) {
  return count_increments(input);
}

Result Day::Part2::process(const Input& input) {
  const auto sums = window_sum(input, 3);
  return count_increments(sums);
}

extern "C" Day* create_object(int num) {
  return new Day();
}

extern "C" void destroy_object(Day* object) {
  delete object;
}