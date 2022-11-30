#pragma once

#include <stdint.h>
#include <string>
#include <vector>

struct Input {
  std::vector<uint16_t> data;
  size_t n_bits;
};
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"