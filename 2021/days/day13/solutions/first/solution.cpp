

#include "solution.hpp"

#include <algorithm>
#include <fstream>

namespace
{
std::vector<Dot> fold_it(std::vector<Dot> dots, const Fold& fold) {
  switch (fold.axis) {
  case Fold::Axis::X: {
    for (auto& dot : dots) {
      if (dot.x > fold.value) {
        dot.x = 2 * fold.value - dot.x;
      }
    }
    dots.erase(
      std::remove_if(dots.begin(), dots.end(), [fold](auto& item) { return item.x == fold.value; }),
      dots.end());
  } break;
  case Fold::Axis::Y: {
    for (auto& dot : dots) {
      if (dot.y > fold.value) {
        dot.y = 2 * fold.value - dot.y;
      }
    }
    dots.erase(
      std::remove_if(dots.begin(), dots.end(), [fold](auto& item) { return item.y == fold.value; }),
      dots.end());
  } break;
  }
  return dots;
}

std::string draw(const std::vector<Dot>& dots) {
  int max_x = std::numeric_limits<int>::min();
  int max_y = std::numeric_limits<int>::min();
  for (const auto& dot : dots) {
    max_x = std::max(max_x, dot.x);
    max_y = std::max(max_y, dot.y);
  }
  std::vector<std::vector<char>> grid(max_y + 1, std::vector<char>(max_x + 1, '.'));
  for (const auto& dot : dots) {
    grid.at(dot.y).at(dot.x) = '#';
  }
  std::ostringstream oss;
  oss << std::endl;
  for (int y = 0; y < grid.size(); y++) {
    for (int x = 0; x < grid.front().size(); x++) {
      oss << grid.at(y).at(x);
    }
    oss << std::endl;
  }
  return oss.str();
}

} // namespace

Input Solution::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    if (line.empty()) {
      break;
    }
    std::string x, y;
    std::stringstream ss(line);
    std::getline(ss, x, ',');
    std::getline(ss, y, ',');
    inputs.first.push_back(Dot{.x = std::stoi(x), .y = std::stoi(y)});
  }
  while (std::getline(line_stream, line)) {
    const auto axis = (line.at(11) == 'x') ? Fold::Axis::X : Fold::Axis::Y;
    inputs.second.push_back(Fold{.axis = axis, .value = std::stoi(line.substr(13))});
  }
  return inputs;
}

Result Solution::Part1::process(const Input& input) {
  std::vector<Dot> dots = fold_it(input.first, input.second.front());
  std::sort(dots.begin(), dots.end());
  auto last = std::unique(dots.begin(), dots.end());
  dots.erase(last, dots.end());
  return std::to_string(dots.size());
}

Result Solution::Part2::process(const Input& input) {
  std::vector<Dot> dots = input.first;
  for (const auto& fold : input.second) {
    dots = fold_it(dots, fold);
  }
  auto result = draw(dots);
  return result;
}

extern "C" Solution* create_solution() {
  return new Solution();
}

extern "C" void destroy_solution(Solution* object) {
  delete object;
}