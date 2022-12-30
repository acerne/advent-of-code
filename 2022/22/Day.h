#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

struct Tile;
struct Instruction;

using Input = std::pair<std::map<int, Tile>, std::vector<Instruction>>;
using Result = int32_t;

static const std::string YEAR{"2022"};
static const std::string DAY{"22"};

static const Result RESULT_PART_ONE{6032};
static const Result RESULT_PART_TWO{5031};

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