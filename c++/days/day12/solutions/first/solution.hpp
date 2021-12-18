#pragma once

#include <stdint.h>
#include <string>
#include <vector>

using Input = std::vector<std::pair<std::string, std::string>>;
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"