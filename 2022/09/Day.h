#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

using Input = std::vector<std::pair<char, int32_t>>;
using Result = size_t;

static const std::string YEAR{"2022"};
static const std::string DAY{"09"};

static const Result RESULT_PART_ONE{13};
static const Result RESULT_PART_TWO{1};

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