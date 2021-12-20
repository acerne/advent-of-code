

#include "solution.hpp"

#include <algorithm>
#include <deque>
#include <fstream>
#include <optional>

namespace
{

int sign(int x) {
  return (x > 0) - (x < 0);
}

bool hit(const Rectangle& target, Coord probe) {
  return probe.x >= target.x1 && probe.x <= target.x2 && probe.y >= target.y1
         && probe.y <= target.y2;
}

Coord highest(const std::deque<Coord>& trajectory) {
  Coord highest{0, 0};
  for (const auto& point : trajectory)
    if (point.y > highest.y) {
      highest = point;
    }

  return highest;
}

void draw(const Rectangle& target, const std::deque<Coord>& trajectory) {
  Rectangle canvas = target;

  for (const auto& point : trajectory) {
    canvas.x1 = std::min(canvas.x1, point.x);
    canvas.x2 = std::max(canvas.x2, point.x);
    canvas.y1 = std::min(canvas.y1, point.y);
    canvas.y2 = std::max(canvas.y2, point.y);
  }

  std::vector<std::vector<char>> grid(canvas.dy() + 1, std::vector<char>(canvas.dx() + 1, '.'));
  for (int y = target.y1; y <= target.y2; y++) {
    for (int x = target.x1; x <= target.x2; x++) {
      grid.at(y - canvas.y1).at(x - canvas.x1) = 'T';
    }
  }
  for (const auto& point : trajectory) {
    grid.at(point.y - canvas.y1).at(point.x - canvas.x1) = '#';
  }

  for (auto row = grid.rbegin(); row != grid.rend(); ++row) {
    for (auto c : *row) {
      std::cout << c;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

std::optional<Coord> estimate(const Rectangle& target, Coord initial_velocity) {
  Coord probe{0, 0};
  Coord velocity(initial_velocity);
  std::deque<Coord> trajectory;
  trajectory.push_back(probe);
  while (probe.y >= target.y1 && probe.x <= target.x2) {
    probe.x += velocity.x;
    probe.y += velocity.y;
    velocity.x -= sign(velocity.x);
    velocity.y--;

    trajectory.push_back(probe);
  }
  // draw(target, trajectory);
  trajectory.pop_back();
  if (hit(target, trajectory.back())) {
    const auto top = highest(trajectory);
    return top;
  } else {
    return {};
  }
}

} // namespace

Input Solution::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line;
  std::getline(line_stream, line);
  std::stringstream ss(line);
  int x1, y1, x2, y2;
  std::string temp;
  std::getline(ss, temp, '=');
  std::getline(ss, temp, '.');
  x1 = std::stoi(temp);
  std::getline(ss, temp, '.');
  std::getline(ss, temp, ',');
  x2 = std::stoi(temp);
  std::getline(ss, temp, '=');
  std::getline(ss, temp, '.');
  y1 = std::stoi(temp);
  std::getline(ss, temp, '.');
  std::getline(ss, temp, ' ');
  y2 = std::stoi(temp);
  inputs.x1 = std::min(x1, x2);
  inputs.y1 = std::min(y1, y2);
  inputs.x2 = std::max(x1, x2);
  inputs.y2 = std::max(y1, y2);
  return inputs;
}

Result Solution::Part1::process(const Input& input) {
  Result best = 0;
  Coord velocity{1, 1};
  for (int vy = 1; vy < 100; vy++) {
    for (int vx = 1; vx < 100; vx++) {
      velocity = {vx, vy};
      auto highest = estimate(input, velocity);
      if (highest) {
        best = std::max(best, highest->y);
      }
    }
  }
  return best;
}

Result Solution::Part2::process(const Input& input) {
  size_t solutions = 0;
  Coord velocity{1, 1};
  for (int vy = -100; vy < 100; vy++) {
    for (int vx = 1; vx < 300; vx++) {
      velocity = {vx, vy};
      auto hit = estimate(input, velocity);
      if (hit) {
        solutions++;
      }
    }
  }
  return solutions;
}

extern "C" Solution* create_solution() {
  return new Solution();
}

extern "C" void destroy_solution(Solution* object) {
  delete object;
}