#pragma once

#include "interfaces.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class DayBase : public IDay
{
public: // Methods
  inline void execute() override final {
    std::cout << "Day " << m_day << ": " << std::endl;

    for (auto& solution : m_solutions) {
      solution->load();
      solution->run();
    }
    std::cout << "===================" << std::endl;
  }

protected: // Members
  DayBase(int day)
    : m_day(day) {}

  std::vector<std::shared_ptr<ISolution>> m_solutions;

private: // Members
  int m_day;
};