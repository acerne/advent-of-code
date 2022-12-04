#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

using Section = std::pair<uint8_t, uint8_t>;

using Input = std::vector<std::pair<Section, Section>>;
using Result = int32_t;

static const std::string YEAR{"2022"};
static const std::string DAY{"04"};

static const Result RESULT_PART_ONE{2};
static const Result RESULT_PART_TWO{4};

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