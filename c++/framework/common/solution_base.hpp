#pragma once

#include "interfaces.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

template<typename, typename = void>
struct is_vector : std::false_type {};

template<typename T>
struct is_vector<std::vector<T>> : std::true_type {};

template<class C>
class SolutionBase : public ISolution
{
public: // Classes
  template<typename T>
  class PartBase : public IPart
  {
  public: // Methods
    virtual T process(const C& input) = 0;

    inline void introduce() override final {
      std::cout << "  Part " << m_part << ": " << std::endl;
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
      const auto report = get_report_(m_result, m_test_result, m_test_expected);
      std::cout << report;
      std::cout << "   Calculation took " << m_duration_us << " us" << std::endl;
    }

  protected: // Methods
    PartBase(int part, const C& input, const C& sample, T sample_result)
      : m_part(part)
      , m_input(input)
      , m_sample(sample)
      , m_test_expected(sample_result) {}

  private: // Methods
    template<typename U>
    static std::string get_report_(const U& result, const U& test_result, const U& test_expected) {
      bool failed = false;
      std::ostringstream oss;
      if (test_result != test_expected) {
        failed = true;
        oss << "     - Expected result = " << test_expected << std::endl;
        oss << "     - Actual result =   " << test_result << std::endl;
      }
      if (failed) {
        oss << "   Test FAILED!" << std::endl;
      } else {
        oss << "   Test passed." << std::endl;
      }
      oss << "   Result = " << result << std::endl;
      return oss.str();
    }

    template<typename U>
    static std::string get_report_(const std::vector<U>& result,
                                   const std::vector<U>& test_result,
                                   const std::vector<U>& test_expected) {
      bool failed = false;
      std::ostringstream oss;
      if (test_result.size() != test_expected.size()) {
        failed = true;
        oss << "     - Expected result size = " << test_expected.size() << std::endl;
        oss << "     - Actual result size =   " << test_result.size() << std::endl;
      } else {
        for (size_t i = 0; i < test_expected.size(); i++) {
          if (test_expected.at(i) > std::numeric_limits<U>::min()) {
            if (test_result.at(i) != test_expected.at(i)) {
              failed = true;
              oss << "     - [" << i << "] Expected result = " << test_expected.at(i) << std::endl;
              oss << "     - [" << i << "] Actual result =   " << test_result.at(i) << std::endl;
            }
          }
        }
      }
      if (failed) {
        oss << "   Test FAILED!" << std::endl;
      } else {
        oss << "   Test passed." << std::endl;
      }
      oss << "   Result = { ";
      for (const auto& value : result) {
        oss << value << " ";
      }
      oss << "}" << std::endl;
      return oss.str();
    }

  private: // Members
    const int m_part;

    const C& m_input;
    const C& m_sample;

    T m_result;
    T m_test_result;
    const T m_test_expected;

    float m_duration_us;
  };

public: // Methods
  virtual C read(const std::string& file_path) = 0;

  inline void load() override final {
    using namespace std::chrono;

    const auto day_id = day_to_string_id(DAY_);
    m_sample = read("../days/" + day_id + "/data/sample.txt");

    const auto load_start = steady_clock::now();
    m_input = read("../days/" + day_id + "/data/input.txt");
    const auto load_end = steady_clock::now();

    const auto duration_ns = duration_cast<nanoseconds>(load_end - load_start);
    m_load_duration_us = duration_ns.count() / 1000.0;
  }

  inline void run() override final {
    std::cout << "----------" << std::endl;
    std::cout << " Solution '" << m_name << "': " << std::endl;
    std::cout << "   Loading took " << m_load_duration_us << " us" << std::endl;

    for (auto& part : m_parts) {
      part->introduce();
      part->test();
      part->run();
      part->print();
    }
  }

protected: // Methods
  SolutionBase(const std::string& name)
    : m_name(name) {}

protected: // Members
  std::vector<std::unique_ptr<IPart>> m_parts;
  C m_input;
  C m_sample;

private: // Members
  std::string m_name;
  float m_load_duration_us;
};