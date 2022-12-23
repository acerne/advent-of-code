#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Solution.h"

namespace aoc {

struct Monkey;

using Input = std::map<std::string, std::shared_ptr<Monkey>>;
using Result = int64_t;

static const std::string YEAR{"2022"};
static const std::string DAY{"21"};

static const Result RESULT_PART_ONE{152};
static const Result RESULT_PART_TWO{301};

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