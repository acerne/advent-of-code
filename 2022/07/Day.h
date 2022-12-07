#pragma once

#include <memory>
#include <vector>

#include "Solution.h"

namespace aoc {

struct Command;

using Input = std::vector<std::shared_ptr<Command>>;
using Result = size_t;

static const std::string YEAR{"2022"};
static const std::string DAY{"07"};

static const Result RESULT_PART_ONE{95437};
static const Result RESULT_PART_TWO{24933642};

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