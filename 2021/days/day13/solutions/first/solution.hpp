#pragma once

#include <stdint.h>
#include <string>
#include <vector>

struct Dot {
  int x;
  int y;

  inline bool operator==(const Dot& other) {
    return other.x == x && other.y == y;
  }
  inline bool operator<(const Dot& other) {
    if (other.y == y) {
      return other.x < x;
    }
    return other.y < y;
  }
};

struct Fold {
  enum class Axis { X, Y } axis;
  int value;
};

using Input = std::pair<std::vector<Dot>, std::vector<Fold>>;
using Result = std::string;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"