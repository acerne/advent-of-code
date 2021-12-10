#pragma once

#include <string>

static std::string day_to_string_id(int day) {
  auto num = std::to_string(day);
  num.insert(0, 1, '0');
  return "day" + num;
}

class IDay
{
public:
  virtual void load() = 0;
  virtual void run() = 0;

protected:
  IDay() {}
};
