#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

using Input = std::vector<std::vector<int32_t>>;
using Result = int32_t;

static const Result RESULT_PART_ONE{24000};
static const Result RESULT_PART_TWO{45000};
static const std::string FOLDER{"../../2022/solutions/day01/"};

class Day01 : public Solution {
  public:  // Constructors
    Day01()
          : Solution(2022, 1) {}

  public:  // Solution methods
    void loadSample() override;
    void loadInput() override;

    void solvePartOne() override;
    bool checkPartOne() const override;
    std::string expectedPartOne() const override;

    void solvePartTwo() override;
    bool checkPartTwo() const override;
    std::string expectedPartTwo() const override;

    std::string result() const override;

  private:  // Members
    Input m_input;
    Result m_result;
};

}  // namespace aoc