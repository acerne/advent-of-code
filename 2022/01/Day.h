#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

using Input = std::vector<std::vector<int32_t>>;
using Result = int32_t;

static const std::string YEAR{"2022"};
static const std::string DAY{"01"};

static const Result RESULT_PART_ONE{24000};
static const Result RESULT_PART_TWO{45000};
static const std::string FOLDER{YEAR + "/" + DAY};

class Day : public Solution<Input, Result> {
  public:  // Constructors
    Day()
          : Solution(FOLDER, RESULT_PART_ONE, RESULT_PART_TWO) {}

  public:  // Solution methods
    Input read(std::ifstream& line_stream) const override;
    Result partOne(const Input& input) const override;
    Result partTwo(const Input& input) const override;
};

}  // namespace aoc