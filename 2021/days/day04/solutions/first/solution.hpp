#pragma once

#include <array>
#include <stdint.h>
#include <string>
#include <vector>

using Board = std::array<std::array<int8_t, 5>, 5>;

struct Input {
  std::vector<int8_t> numbers;
  std::vector<Board> boards;
};
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"