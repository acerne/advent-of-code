#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

using Rules = std::map<std::pair<char, char>, char>;

struct Input {
    std::string polymer_template;
    Rules insertion_rules;
};

using Result = size_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"14"};

static const Result RESULT_PART_ONE{1588};
static const Result RESULT_PART_TWO{2188189693529};

class Day : public Solution<Input, Result> {
  public:  // Constructors
    Day()
          : Solution(YEAR, DAY, RESULT_PART_ONE, RESULT_PART_TWO) {}

  public:  // Solution methods
    Input read(std::ifstream& line_stream) const override;
    Result partOne(const Input& input) const override;
    Result partTwo(const Input& input) const override;
};

}  // namespace aoc