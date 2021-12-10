#pragma once

#include "base.hpp"

class Day : public DayBase<Input>
{
public:
  class Part1 : public DayBase::PartBase<Result>
  {
  public:
    Part1(const Input& input, const Input& sample)
      : PartBase(1, input, sample, PART1_SAMPLE_RESULT_) {}

    Result process(const Input& input) override;
  };

  class Part2 : public DayBase::PartBase<Result>
  {
  public:
    Part2(const Input& input, const Input& sample)
      : PartBase(2, input, sample, PART2_SAMPLE_RESULT_) {}

    Result process(const Input& input) override;
  };

public:
  Day()
    : DayBase(DAY_) {
    m_parts.push_back(std::make_unique<Part1>(m_input, m_sample));
    m_parts.push_back(std::make_unique<Part2>(m_input, m_sample));
  }
  Day(const Day&) = delete;
  Day(Day&&) = delete;

  void load() override;
};
