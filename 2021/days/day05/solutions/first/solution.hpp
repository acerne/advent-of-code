#pragma once

#include <stdint.h>
#include <string>
#include <vector>

struct Line {
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2;
};

using Input = std::vector<Line>;
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"