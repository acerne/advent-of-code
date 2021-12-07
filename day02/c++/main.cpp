#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum class Command { FORWARD, UP, DOWN };

static const std::map<std::string, Command> string_to_command_{
    {"forward", Command::FORWARD},
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

std::vector<std::pair<Command, int32_t>>
read_values(const std::string &file_path) {
  std::vector<std::pair<Command, int32_t>> values;
  std::ifstream line_stream(file_path);
  std::string line, command, value;
  while (std::getline(line_stream, line)) {
    std::stringstream ss(line);
    getline(ss, command, ' ');
    getline(ss, value, ' ');
    values.emplace_back(string_to_command_.at(command), std::stoi(value));
  }
  return values;
}

int main(int argc, char *argv[]) {

  if (argc > 1) {
    const auto file_path = std::string(argv[1]);
    const auto values = read_values(file_path);

    { // Challenge 1
      Submarine1 sub{0, 0};

      for (const auto &[command, value] : values) {
        sub.move(command, value);
      }

      std::cout << "Final position (challenge 1): " << std::endl;

      std::cout << " horizontal : " << sub.m_horizontal << std::endl;
      std::cout << " depth      : " << sub.m_depth << std::endl;
      std::cout << " result     : " << sub.m_horizontal * sub.m_depth
                << std::endl;
    }

    { // Challenge 2
      Submarine2 sub{0, 0, 0};

      for (const auto &[command, value] : values) {
        sub.move(command, value);
      }

      std::cout << "Final position (challenge 2): " << std::endl;

      std::cout << " horizontal : " << sub.m_horizontal << std::endl;
      std::cout << " depth      : " << sub.m_depth << std::endl;
      std::cout << " result     : " << sub.m_horizontal * sub.m_depth
                << std::endl;
    }
  } else {
    std::cerr << "Missing argument." << std::endl;
  }

  return 0;
}