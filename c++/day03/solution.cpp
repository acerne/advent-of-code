#include "datatypes.hpp"
#include "day.hpp"

#include <fstream>
#include <sstream>

namespace
{
std::array<size_t, 12> count(const std::vector<std::bitset<12>>& values) {
  std::array<size_t, 12> true_count{};
  for (const auto& item : values) {
    for (size_t i = 0; i < 12; i++) {
      if (item[i]) {
        true_count[i]++;
      }
    }
  }
  return true_count;
}

std::pair<int16_t, int16_t> decode(const std::array<size_t, 12>& true_count, size_t total) {
  std::bitset<12> gamma = 0;
  for (size_t i = 0; i < 12; i++) {
    if (true_count[i] > total / 2) {
      gamma.set(i);
    }
  }
  std::bitset<12> epsilon = ~gamma;
  return {static_cast<int16_t>(gamma.to_ulong()), static_cast<int16_t>(epsilon.to_ulong())};
}

} // namespace

void Day::load() {
  {
    std::ifstream line_stream("../" + day_to_string_id(DAY_) + "/sample.txt");
    std::string line;
    while (std::getline(line_stream, line)) {
      m_input.push_back(std::bitset<12>(std::stoi(line, nullptr, 2)));
    }
  }
  {
    std::ifstream line_stream("../" + day_to_string_id(DAY_) + "/data.txt");
    std::string line;
    while (std::getline(line_stream, line)) {
      m_input.push_back(std::bitset<12>(std::stoi(line, nullptr, 2)));
    }
  }
}

Result Day::Part1::process(const Input& input) {
  const auto counts = count(input);
  const auto [gamma, epsilon] = decode(counts, input.size());
  return gamma * epsilon;
}

Result Day::Part2::process(const Input& input) {
  return -1;
}

extern "C" Day* create_object(int num) {
  return new Day();
}

extern "C" void destroy_object(Day* object) {
  delete object;
}
