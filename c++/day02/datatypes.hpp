#pragma once

#include <vector>

enum class Command { FORWARD, UP, DOWN };

using Input = std::vector<std::pair<Command, int32_t>>;
using Result = int32_t;

static int DAY_ = 2;
static Result PART1_SAMPLE_RESULT_ = 150;
static Result PART2_SAMPLE_RESULT_ = 900;