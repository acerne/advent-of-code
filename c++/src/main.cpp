#include "days.hpp"

#include <memory>

int main() {
  std::vector<std::unique_ptr<IDay>> days;
  days.push_back(std::make_unique<Day01>());

  for (const auto& day : days) {
    day->load();
    day->run();
  }
}