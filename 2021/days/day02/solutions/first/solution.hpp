#pragma once

#include <stdint.h>
#include <string>
#include <vector>

enum class Command { FORWARD, UP, DOWN };

using Input = std::vector<std::pair<Command, int32_t>>;
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"