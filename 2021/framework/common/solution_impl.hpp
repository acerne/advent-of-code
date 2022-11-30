#pragma once

#include "solution_base.hpp"

class Solution : public SolutionBase<Input>
{
public: // Classes
  class Part1 : public SolutionBase::PartBase<Result>
  {
  public: // Methods
    Part1(const Input& input, const Input& sample)
      : PartBase(1, input, sample, PART1_SAMPLE_RESULT_) {}

    Result process(const Input& input) override;
  };

  class Part2 : public SolutionBase::PartBase<Result>
  {
  public: // Methods
    Part2(const Input& input, const Input& sample)
      : PartBase(2, input, sample, PART2_SAMPLE_RESULT_) {}

    Result process(const Input& input) override;
  };

public: // Methods
  Solution()
    : SolutionBase(SOLUTION_NAME_) {
    m_parts.push_back(std::make_unique<Part1>(m_input, m_sample));
    m_parts.push_back(std::make_unique<Part2>(m_input, m_sample));
  }
  Solution(const Solution&) = delete;
  Solution(Solution&&) = delete;

  Input read(const std::string& file_path) override;
};
