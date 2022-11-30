#include "Solution.h"

namespace aoc {

Solution::Solution(int year, int day)
      : m_year(year),
        m_day(day) {}

int Solution::year() const {
    return m_year;
}

int Solution::day() const {
    return m_day;
}

}  // namespace aoc