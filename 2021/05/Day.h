#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

struct Line {
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};

using Input = std::vector<Line>;
using Result = int32_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"05"};

static const Result RESULT_PART_ONE{5};
static const Result RESULT_PART_TWO{12};

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