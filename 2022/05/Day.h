#pragma once

#include <deque>
#include <map>
#include <vector>

#include "Solution.h"

namespace aoc {

using Cargo = std::map<int, std::deque<char>>;

struct Move {
    int count;
    int from;
    int to;
};

using Input = std::pair<Cargo, std::deque<Move>>;
using Result = std::string;

static const std::string YEAR{"2022"};
static const std::string DAY{"05"};

static const Result RESULT_PART_ONE{"CMZ"};
static const Result RESULT_PART_TWO{"MCD"};

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