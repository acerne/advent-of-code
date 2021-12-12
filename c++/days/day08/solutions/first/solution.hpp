#pragma once

#include <array>
#include <stdint.h>
#include <string>
#include <vector>

struct Signal {
  std::array<std::string, 10> in;
  std::array<std::string, 4> out;
};

using Input = std::vector<Signal>;
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"