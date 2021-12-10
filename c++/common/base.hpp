#pragma once

#include "interface.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class IPart
{
public:
  virtual void introduce() = 0;
  virtual void process() = 0;
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
    inline void introduce() override {
      std::cout << " Part " << m_part << ": " << std::endl;
    }
    inline void print() override {
      std::cout << "   Result = " << m_result << ": " << std::endl;
    }

  protected:
    PartBase(int part)
      : m_part(part) {}

  protected:
    T m_result;

  private:
    int m_part;
  };

  inline void run() override final {
    std::cout << "Day " << m_day << ": " << std::endl;

    for (auto& part : m_parts) {
      part->introduce();
      part->process();
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

private:
  int m_day;
};
