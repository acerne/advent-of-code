

#include "solution.hpp"

#include <fstream>

namespace
{} // namespace

Input Solution::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    // inputs.push_back( );
  }
  return inputs;
}

Result Solution::Part1::process(const Input& input) {
  // return ;
}

Result Solution::Part2::process(const Input& input) {
  // return ;
}

extern "C" Solution* create_solution() {
  return new Solution();
}

extern "C" void destroy_solution(Solution* object) {
  delete object;
}