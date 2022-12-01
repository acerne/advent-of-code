#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

using Input = std::vector<std::vector<int8_t>>;
using Result = int32_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"09"};

static const Result RESULT_PART_ONE{15};
static const Result RESULT_PART_TWO{1134};

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