

#include "datatypes.hpp"
#include "day.hpp"

#include <fstream>
#include <numeric>

namespace
{} // namespace

void Day::load() {
  std::ifstream line_stream("../" + day_to_string_id(DAY_) + "/data.txt");
  std::string line;
  while (std::getline(line_stream, line)) {
    // m_input.push_back(
  }
}

void Day::Part1::process() {
  // m_result =
}

void Day::Part2::process() {
  // m_result =
}

extern "C" Day* create_object(int num) {
  return new Day(num);
}

extern "C" void destroy_object(Day* object) {
  delete object;
}