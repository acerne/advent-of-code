#pragma once

#include "base.hpp"

class Day : public DayBase<Input>
{
public:
  static std::string to_string_id(int day) {
    auto num = std::to_string(day);
    num.insert(0, 1, '0');
    return "day" + num;
  }

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
  Day()
    : DayBase(DAY_) {
    m_parts.push_back(std::make_unique<Part1>(m_input));
    m_parts.push_back(std::make_unique<Part2>(m_input));
  }
  Day(const Day&) = delete;
  Day(Day&&) = delete;

  void load() override;
};