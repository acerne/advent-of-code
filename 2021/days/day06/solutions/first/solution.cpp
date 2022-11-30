

#include "solution.hpp"

#include <deque>
#include <fstream>
#include <numeric>
#include <sstream>

namespace
{
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

} // namespace

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
  return simulate(input, 80);
}

Result Solution::Part2::process(const Input& input) {
  return simulate(input, 256);
}

extern "C" Solution* create_solution() {
  return new Solution();
}

extern "C" void destroy_solution(Solution* object) {
  delete object;
}