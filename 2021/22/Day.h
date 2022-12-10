#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

struct Instruction {
    bool on;
    int32_t x1, x2;
    int32_t y1, y2;
    int32_t z1, z2;
};

using Input = std::vector<Instruction>;
using Result = size_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"22"};

static const Result RESULT_PART_ONE{474140};
static const Result RESULT_PART_TWO{2758514936282235};

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