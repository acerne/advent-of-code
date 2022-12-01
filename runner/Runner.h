#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>

#include "Solution.h"

namespace aoc {

class Runner {
  public:  // Constructors
    explicit Runner(int argc, char* argv[]);

  public:  // Methods
    void load();
    void run();

  private:  // Members
    std::optional<std::string> m_name;
    std::map<std::string, std::unique_ptr<SolutionBase>> m_solutions;
};
}  // namespace aoc