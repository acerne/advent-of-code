#pragma once

#include <functional>
#include <vector>

#include "Solution.h"

namespace aoc {

struct Monkey {
    std::vector<size_t> worry_levels;
    std::function<size_t(size_t)> operation;
    std::function<size_t(size_t)> test;
    size_t divisor;
    size_t n_inspections{0};
};

using Input = std::vector<Monkey>;
using Result = size_t;

static const std::string YEAR{"2022"};
static const std::string DAY{"11"};

static const Result RESULT_PART_ONE{10605};
static const Result RESULT_PART_TWO{2713310158};

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