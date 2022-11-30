#pragma once

#include <map>
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

struct Input {
  std::map<Coord, int8_t> risk;
  Coord start;
  Coord stop;
};
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"