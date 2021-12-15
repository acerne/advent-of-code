#pragma once

#include <array>
#include <stdint.h>
#include <string>
#include <vector>

using Input = std::array<std::array<int8_t, 10>, 10>;
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"