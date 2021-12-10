#pragma once

#include <stdint.h>
#include <vector>

struct Input {
  std::vector<uint16_t> data;
  size_t n_bits;
};
using Result = int32_t;

static int DAY_ = 3;
static Result PART1_SAMPLE_RESULT_ = 198;
static Result PART2_SAMPLE_RESULT_ = 230;