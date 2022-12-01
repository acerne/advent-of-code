#pragma once

#include <vector>

#include "Solution.h"

namespace aoc {

struct Dot {
    int x;
    int y;

    inline bool operator==(const Dot& other) {
        return other.x == x && other.y == y;
    }

    inline bool operator<(const Dot& other) {
        if (other.y == y) {
            return other.x < x;
        }
        return other.y < y;
    }
};

struct Fold {
    enum class Axis {
        X,
        Y
    } axis;
    int value;
};

using Input = std::pair<std::vector<Dot>, std::vector<Fold>>;
using Result = std::string;

static const std::string YEAR{"2021"};
static const std::string DAY{"13"};

static const Result RESULT_PART_ONE{"17"};
static const Result RESULT_PART_TWO{"\n#####\n#...#\n#...#\n#...#\n#####\n"};

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