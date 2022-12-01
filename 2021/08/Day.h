#pragma once

#include <array>
#include <vector>

#include "Solution.h"

namespace aoc {

struct Signal {
    std::array<std::string, 10> in;
    std::array<std::string, 4> out;
};

using Input = std::vector<Signal>;
using Result = int32_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"08"};

static const Result RESULT_PART_ONE{26};
static const Result RESULT_PART_TWO{61229};

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