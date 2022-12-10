#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

struct Instruction {
    bool addx;
    int32_t value{0};
};

using Input = std::vector<Instruction>;
using Result = std::string;

static const std::string YEAR{"2022"};
static const std::string DAY{"10"};

static const Result RESULT_PART_ONE{"13140"};
static const Result RESULT_PART_TWO{"##..##..##..##..##..##..##..##..##..##..\n"
                                    "###...###...###...###...###...###...###.\n"
                                    "####....####....####....####....####....\n"
                                    "#####.....#####.....#####.....#####.....\n"
                                    "######......######......######......####\n"
                                    "#######.......#######.......#######.....\n"};

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