#pragma once

#include "base.hpp"

using Input = int32_t;
using Result = size_t;

class Day01 : public Day<Input>
{
  class Part1 : public Day::Part<Result>
  {
  public:
    Part1(const Day01& day);

    void process() override;

  private:
    const Day01& m_day;
  };

  class Part2 : public Day::Part<Result>
  {
  public:
    Part2(const Day01& day);

    void process() override;

  private:
    const Day01& m_day;
  };

public:
  Day01();

  void load() override;

private:
  std::string m_path;
};
