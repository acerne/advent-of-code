

#include "solution.hpp"

#include <fstream>
#include <sstream>

namespace
{} // namespace

Input Solution::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line, number;
  while (std::getline(line_stream, line)) {
    std::stringstream ss(line);
    Line obj;
    getline(ss, number, ',');
    obj.x1 = std::stoi(number);
    getline(ss, number, ' ');
    obj.y1 = std::stoi(number);
    getline(ss, number, ' ');
    getline(ss, number, ',');
    obj.x2 = std::stoi(number);
    getline(ss, number, ' ');
    obj.y2 = std::stoi(number);
    inputs.push_back(obj);
  }
  return inputs;
}

Result Solution::Part1::process(const Input& input) {
  std::array<std::array<int16_t, 1000>, 1000> grid{};
  for (const auto& line : input) {
    if (line.x1 == line.x2) { // vertical
      const auto x = line.x1;
      for (int16_t y = std::min(line.y1, line.y2); y <= std::max(line.y1, line.y2); y++) {
        grid[x][y]++;
      }
    } else if (line.y1 == line.y2) { // horizontal
      const auto y = line.y1;
      for (int16_t x = std::min(line.x1, line.x2); x <= std::max(line.x1, line.x2); x++) {
        grid[x][y]++;
      }
    }
  }
  size_t count = 0;
  for (int16_t y = 0; y < 1000; y++) {
    for (int16_t x = 0; x < 1000; x++) {
      if (grid[x][y] >= 2) {
        count++;
      }
    }
  }
  return count;
}

Result Solution::Part2::process(const Input& input) {
  std::array<std::array<int16_t, 1000>, 1000> grid{};
  for (const auto& line : input) {
    int16_t dx = (line.x2 == line.x1) ? 0 : (line.x2 > line.x1) ? 1 : -1;
    int16_t dy = (line.y2 == line.y1) ? 0 : (line.y2 > line.y1) ? 1 : -1;

    int16_t x = line.x1;
    int16_t y = line.y1;
    grid[x][y]++;
    do {
      x += dx;
      y += dy;
      grid[x][y]++;
    } while (x != line.x2 || y != line.y2);
  }

  size_t count = 0;
  for (int16_t y = 0; y < 1000; y++) {
    for (int16_t x = 0; x < 1000; x++) {
      if (grid[x][y] >= 2) {
        count++;
      }
    }
  }
  return count;
}

extern "C" Solution* create_solution() {
  return new Solution();
}

extern "C" void destroy_solution(Solution* object) {
  delete object;
}