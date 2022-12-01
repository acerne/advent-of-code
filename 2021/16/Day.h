#pragma once

#include <limits>
#include <vector>

#include "Solution.h"

namespace aoc {

static const uint64_t NUL = std::numeric_limits<uint64_t>::max();

using Input = std::vector<std::string>;
using Result = std::vector<uint64_t>;

static const std::string YEAR{"2021"};
static const std::string DAY{"16"};

static const Result RESULT_PART_ONE{16, 12, 23, 31, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL};
static const Result RESULT_PART_TWO{NUL, NUL, NUL, NUL, 3, 54, 7, 9, 1, 0, 0, 1};

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