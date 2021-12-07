#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class IDay
{
public:
  virtual void load() = 0;
  virtual void run() = 0;

protected:
  IDay() {}
};

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
class Day : public IDay
{
public:
  template<typename T>
  class Part : public IPart
  {
  public:
    inline void introduce() override final {
      std::cout << " Part " << m_part << ": " << std::endl;
    }
    inline void print() override final {
      std::cout << "  Result = " << m_result << ": " << std::endl;
      std::cout << std::endl;
    }

  protected:
    Part(int part)
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
  }

protected:
  Day(int day)
    : m_day(day) {}

protected:
  std::vector<std::unique_ptr<IPart>> m_parts;

  std::vector<C> m_inputs;

private:
  int m_day;
};
