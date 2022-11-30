#pragma once

#include <stdint.h>
#include <string>
#include <vector>

struct Coord {
  int x;
  int y;

  inline bool operator==(const Coord& other) const {
    return x == other.x && y == other.y;
  }
  inline bool operator!=(const Coord& other) const {
    return !operator==(other);
  }
  inline bool operator<(const Coord& other) const {
    return y < other.y || (y == other.y && x < other.x);
  }
};

struct Rectangle {
  int x1;
  int y1;
  int x2;
  int y2;

  int dx() {
    return x2 - x1;
  };
  int dy() {
    return y2 - y1;
  };
};

using Input = Rectangle;
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"