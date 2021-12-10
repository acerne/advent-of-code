#include "datatypes.hpp"
#include "day.hpp"

#include <fstream>
#include <sstream>

namespace
{

std::vector<bool> parse_line(const std::string& line) {
  std::vector<bool> bits;
  for (const auto& c : line) {
    bits.push_back(c != '0');
  }
  std::reverse(bits.begin(), bits.end());
  return bits;
}

std::vector<size_t> count(const Input& values) {
  size_t n_bits = values.front().size();
  std::vector<size_t> true_count(n_bits, 0);
  for (const auto& item : values) {
    for (size_t i = 0; i < n_bits; i++) {
      if (item.at(i)) {
        true_count.at(i)++;
      }
    }
  }
  return true_count;
}

std::pair<int16_t, int16_t> decode(const std::vector<size_t>& true_count, size_t total) {
  size_t n_bits = true_count.size();
  int16_t gamma = 0;
  for (size_t i = 0; i < n_bits; i++) {
    if (true_count.at(i) > total / 2) {
      gamma |= 1 << i;
    }
  }
  int16_t epsilon = ~(gamma | (0xFFFF << n_bits));
  return {gamma, epsilon};
}

} // namespace

Input Day::read(const std::string& file_path) {
  Input data;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    data.push_back(parse_line(line));
  }
  return data;
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
