#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::array<size_t, 12> count(const std::vector<std::bitset<12>> &values) {
  std::array<size_t, 12> true_count{};
  for (const auto &item : values) {
    for (size_t i = 0; i < 12; i++) {
      if (item[i]) {
        true_count[i]++;
      }
    }
  }
  return true_count;
}

std::pair<int16_t, int16_t> decode(const std::array<size_t, 12> &true_count,
                                   size_t total) {
  std::bitset<12> gamma = 0;
  for (size_t i = 0; i < 12; i++) {
    if (true_count[i] > total / 2) {
      gamma.set(i);
    }
  }
  std::bitset<12> epsilon = ~gamma;
  return {static_cast<int16_t>(gamma.to_ulong()),
          static_cast<int16_t>(epsilon.to_ulong())};
}

std::vector<std::bitset<12>> read_values(const std::string &file_path) {
  std::vector<std::bitset<12>> values;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    values.push_back(std::bitset<12>(std::stoi(line, nullptr, 2)));
  }
  return values;
}

int main(int argc, char *argv[]) {

  if (argc > 1) {
    const auto file_path = std::string(argv[1]);
    const auto values = read_values(file_path);
    const auto counts = count(values);
    const auto [gamma, epsilon] = decode(counts, values.size());

    std::cout << "Gamma:   " << gamma << "\t - \t" << std::bitset<16>(gamma)
              << std::endl;
    std::cout << "Epsilon: " << epsilon << "\t - \t" << std::bitset<16>(epsilon)
              << std::endl;
    std::cout << "Result:  " << gamma * epsilon << std::endl;
  } else {
    std::cerr << "Missing argument." << std::endl;
  }

  return 0;
}