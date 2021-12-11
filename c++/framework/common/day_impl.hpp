#pragma once

#include "day_base.hpp"
#include "loader.hpp"

class Day : DayBase
{
public:
  Day()
    : DayBase(DAY_) {
    m_solutions = SolutionLoader::load_all(DAY_);
  }
};
