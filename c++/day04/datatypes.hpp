#pragma once

#include <array>
#include <vector>

using Board = std::array<std::array<int8_t, 5>, 5>;

struct Input {
  std::vector<int8_t> numbers;
  std::vector<Board> boards;
};
using Result = int32_t;

static int DAY_ = 4;
static Result PART1_SAMPLE_RESULT_ = 4512;
static Result PART2_SAMPLE_RESULT_ = 1924;