

#include "datatypes.hpp"
#include "day.hpp"

#include <fstream>

namespace
{} // namespace

Input Day::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    // inputs.push_back();
  }
  return inputs;
}

Result Day::Part1::process(const Input& input) {
  return -1;
}

Result Day::Part2::process(const Input& input) {
  return -1;
}

extern "C" Day* create_object(int num) {
  return new Day();
}

extern "C" void destroy_object(Day* object) {
  delete object;
}