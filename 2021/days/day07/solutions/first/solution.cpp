

#include "solution.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace
{} // namespace

Input Solution::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line, number;
  std::getline(line_stream, line);
  std::stringstream ss(line);
  while (std::getline(ss, number, ',')) {
    inputs.push_back(std::stoi(number));
  }
  return inputs;
}

Result Solution::Part1::process(const Input& input) {
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

Result Solution::Part2::process(const Input& input) {
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

extern "C" Solution* create_solution() {
  return new Solution();
}

extern "C" void destroy_solution(Solution* object) {
  delete object;
}