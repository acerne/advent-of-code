#pragma once

#include "interface.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class IPart
{
public:
  virtual void introduce() = 0;
  virtual void test() = 0;
  virtual void run() = 0;
  virtual void print() = 0;

protected:
  IPart() {}
};

template<class C>
class DayBase : public IDay
{
public:
  template<typename T>
  class PartBase : public IPart
  {
  public:
    virtual T process(const C& input) = 0;

    inline void introduce() override final {
      std::cout << " Part " << m_part << ": " << std::endl;
    }
    inline void test() override final {
      m_test_result = process(m_sample);
    }
    inline void run() override final {
      const auto start = std::chrono::steady_clock::now();
      m_result = process(m_input);
      const auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now() - start);
      m_duration_us = duration_ns.count() / 1000.0;
    }
    inline void print() override final {
      if (m_test_result != m_test_expected) {
        std::cout << "   Test FAILED!" << std::endl;
        std::cout << "     - Expected result = " << m_test_expected << std::endl;
        std::cout << "     - Actual result =   " << m_test_result << std::endl;
      } else {
        std::cout << "   Test passed." << std::endl;
      }
      std::cout << "   Result = " << m_result << std::endl;
      std::cout << "   Calculation took " << m_duration_us << " us" << std::endl;
    }

  protected:
    PartBase(int part, const Input& input, const Input& sample, Result sample_result)
      : m_part(part)
      , m_input(input)
      , m_sample(sample)
      , m_test_expected(sample_result) {}

  private:
    const int m_part;

    const Input& m_input;
    const Input& m_sample;

    T m_result;
    T m_test_result;
    const T m_test_expected;

    float m_duration_us;
  };

public:
  virtual C read(const std::string& file_path) = 0;

  inline void load() override final {
    m_sample = read("../" + day_to_string_id(DAY_) + "/sample.txt");
    const auto start = std::chrono::steady_clock::now();
    m_input = read("../" + day_to_string_id(DAY_) + "/data.txt");
    const auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::steady_clock::now() - start);
    m_load_duration_us = duration_ns.count() / 1000.0;
  }
  inline void run() override final {
    std::cout << "Day " << m_day << ": " << std::endl;
    std::cout << "  Loading took " << m_load_duration_us << " us" << std::endl;

    for (auto& part : m_parts) {
      part->introduce();
      part->test();
      part->run();
      part->print();
    }
    std::cout << "-----------------" << std::endl;
  }

protected:
  DayBase(int day)
    : m_day(day) {}

protected:
  std::vector<std::unique_ptr<IPart>> m_parts;

  C m_input;
  C m_sample;

private:
  int m_day;

  float m_load_duration_us;
};
