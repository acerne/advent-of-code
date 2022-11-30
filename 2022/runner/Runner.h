#pragma once

#include <optional>

#include "FactoryManager.h"

namespace aoc {

class Runner {
  public:  // Constructors
    explicit Runner(int argc, char* argv[]);

  public:  // Methods
    void load();
    void run();

  private:  // Members
    std::optional<int> m_year;
    std::optional<int> m_day;
    std::optional<std::string> m_name;
    std::map<std::string, std::unique_ptr<Solution>> m_solutions;
};
}  // namespace aoc