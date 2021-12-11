
#include "day.hpp"

#include "day_impl.hpp"

extern "C" Day* create_day() {
  return new Day();
}

extern "C" void destroy_day(Day* object) {
  delete object;
}
