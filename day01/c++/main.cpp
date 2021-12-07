#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<int32_t> read_values(const std::string &file_path) {
  std::vector<int32_t> values;
  std::ifstream line_stream(file_path);
  std::string line;
  while (std::getline(line_stream, line)) {
    values.push_back(std::stoi(line));
  }
  return values;
}

size_t count_increments(const std::vector<int32_t> &values) {
  size_t increments = 0;
  for (size_t i = 1; i < values.size(); i++) {
    if (values.at(i) > values.at(i - 1)) {
      increments++;
    }
  }
  return increments;
}

std::vector<int32_t> window_sum(const std::vector<int32_t> &inputs,
                                size_t window_size) {
  std::vector<int32_t> sums;
  int32_t rolling_sum =
      std::accumulate(inputs.begin(), inputs.begin() + window_size, 0);
  sums.push_back(rolling_sum);
  for (size_t i = window_size; i < inputs.size(); i++) {
    rolling_sum -= inputs.at(i - window_size);
    rolling_sum += inputs.at(i);
    sums.push_back(rolling_sum);
  }
  return sums;
}

int main(int argc, char *argv[]) {

  if (argc > 1) {
    const auto file_path = std::string(argv[1]);
    const auto values = read_values(file_path);
    const auto increments = count_increments(values);

    std::cout << "Number of increments: " << increments << std::endl;

    const auto sums = window_sum(values, 3);
    const auto sum_increments = count_increments(sums);

    std::cout << "Number of sum increments: " << sum_increments << std::endl;
  } else {
    std::cerr << "Missing argument." << std::endl;
  }

  return 0;
}