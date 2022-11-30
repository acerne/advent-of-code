#pragma once

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

using Rules = std::map<std::pair<char, char>, char>;

struct Input {
  std::string polymer_template;
  Rules insertion_rules;
};
using Result = size_t;

static const std::string SOLUTION_NAME_{"first"};

#include "day.hpp"
#include "result.hpp"
#include "solution_impl.hpp"