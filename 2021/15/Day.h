#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

struct Coord {
    int x;
    int y;

    inline bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }

    inline bool operator!=(const Coord& other) const {
        return !operator==(other);
    }

    inline bool operator<(const Coord& other) const {
        return y < other.y || (y == other.y && x < other.x);
    }
};

struct Input {
    std::map<Coord, int8_t> risk;
    Coord start;
    Coord stop;
};

using Result = int32_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"15"};

static const Result RESULT_PART_ONE{40};
static const Result RESULT_PART_TWO{315};

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