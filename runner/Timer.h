#pragma once

#include <chrono>

namespace aoc {

using ClockType = std::chrono::steady_clock;
using TimeUnit = std::chrono::microseconds;

class Timer {
  public:  // Constructors
    Timer() {}

    void start() {
        m_start = ClockType::now();
    }

    void stop() {
        m_stop = ClockType::now();
    }

    double duration() const {
        return std::chrono::duration_cast<TimeUnit>(m_stop - m_start).count() / 1000.0;
    }

  public:  // Methods

  private:  // Members
    std::chrono::time_point<ClockType> m_start;
    std::chrono::time_point<ClockType> m_stop;
};

}  // namespace aoc