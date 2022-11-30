#pragma once

#include <string>

namespace aoc {

class Solution {
  public:  // Constructors
    Solution(int year, int day);
    virtual ~Solution() = default;

  public:  // Methods
    int year() const;
    int day() const;

  public:  // Pure virtual methods
    virtual void loadSample() = 0;
    virtual void loadInput() = 0;

    virtual void solvePartOne() = 0;
    virtual bool checkPartOne() const = 0;
    virtual std::string expectedPartOne() const = 0;

    virtual void solvePartTwo() = 0;
    virtual bool checkPartTwo() const = 0;
    virtual std::string expectedPartTwo() const = 0;

    virtual std::string result() const = 0;

  private:  // Members
    const int m_year;
    const int m_day;
    const std::string m_name;
};

}  // namespace aoc