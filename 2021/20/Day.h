#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

struct Image {
    std::string pixels;
    size_t width{0};
    size_t height{0};
    char outside;
};

using Algorithm = std::string;

using Input = std::pair<Algorithm, Image>;
using Result = int32_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"20"};

static const Result RESULT_PART_ONE{35};
static const Result RESULT_PART_TWO{3351};

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