#pragma once

#include <stdint.h>
#include <string>
#include <vector>

class Snailfish;

using Input = std::vector<Snailfish>;
using Result = int32_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"