#pragma once

#include "base.hpp"

class Day : public DayBase<Input>
{
public:
  class Part1 : public DayBase::PartBase<Result>
  {
  public:
    Part1(const Input& input)
      : PartBase(1)
      , m_input(input) {}

    void process() override;

  private:
    const Input& m_input;
  };

  class Part2 : public DayBase::PartBase<Result>
  {
  public:
    Part2(const Input& input)
      : PartBase(2)
      , m_input(input) {}

    void process() override;

  private:
    const Input& m_input;
  };

public:
  Day(int num)
    : DayBase(num) {
    m_parts.push_back(std::make_unique<Part1>(m_input));
    m_parts.push_back(std::make_unique<Part2>(m_input));
  }
  Day() = delete;
  Day(const Day&) = delete;
  Day(Day&&) = delete;

  void load() override;
};
