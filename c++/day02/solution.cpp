#include "datatypes.hpp"
#include "day.hpp"

#include <fstream>
#include <map>
#include <numeric>
#include <sstream>

namespace
{

static const std::map<std::string, Command> string_to_command_{{"forward", Command::FORWARD},
                                                               {"up", Command::UP},
                                                               {"down", Command::DOWN}};

struct Submarine1 {
  int32_t m_horizontal;
  int32_t m_depth;

  void move(Command command, int32_t value) {
    switch (command) {
    case Command::FORWARD:
      m_horizontal += value;
      break;
    case Command::UP:
      m_depth -= value;
      break;
    case Command::DOWN:
      m_depth += value;
      break;
    }
  }
};

struct Submarine2 {
  int32_t m_horizontal;
  int32_t m_depth;
  int32_t m_aim;

  void move(Command command, int32_t value) {
    switch (command) {
    case Command::FORWARD:
      m_horizontal += value;
      m_depth += value * m_aim;
      break;
    case Command::UP:
      m_aim -= value;
      break;
    case Command::DOWN:
      m_aim += value;
      break;
    }
  }
};
} // namespace

Input Day::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream("../" + day_to_string_id(DAY_) + "/sample.txt");
  std::string line, command, value;
  while (std::getline(line_stream, line)) {
    std::stringstream ss(line);
    getline(ss, command, ' ');
    getline(ss, value, ' ');
    inputs.emplace_back(string_to_command_.at(command), std::stoi(value));
  }
  return inputs;
}

Result Day::Part1::process(const Input& input) {
  Submarine1 sub{0, 0};
  for (const auto& [command, value] : input) {
    sub.move(command, value);
  }
  return sub.m_horizontal * sub.m_depth;
}

Result Day::Part2::process(const Input& input) {
  Submarine2 sub{0, 0, 0};
  for (const auto& [command, value] : input) {
    sub.move(command, value);
  }
  return sub.m_horizontal * sub.m_depth;
}

extern "C" Day* create_object(int num) {
  return new Day();
}

extern "C" void destroy_object(Day* object) {
  delete object;
}
