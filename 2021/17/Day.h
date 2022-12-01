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

struct Rectangle {
    int x1;
    int y1;
    int x2;
    int y2;

    int dx() {
        return x2 - x1;
    };

    int dy() {
        return y2 - y1;
    };
};

using Input = Rectangle;
using Result = int32_t;

static const std::string YEAR{"2021"};
static const std::string DAY{"17"};

static const Result RESULT_PART_ONE{45};
static const Result RESULT_PART_TWO{112};

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