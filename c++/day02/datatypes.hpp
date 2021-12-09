#pragma once

#include <vector>

enum class Command { FORWARD, UP, DOWN };

using Input = std::vector<std::pair<Command, int32_t>>;
using Result = size_t;

static int DAY_ = 2;