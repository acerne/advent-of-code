#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>
#include <bitset>
#include <numeric>
#include <sstream>

namespace aoc {

namespace {

uint16_t parse_line(const std::string& line) {
    std::bitset<16> bits(std::stoi(line, nullptr, 2));
    return bits.to_ulong();
}

std::pair<size_t, size_t> count_bit(const Input& input, size_t bit) {
    auto trues = std::count_if(input.data.begin(), input.data.end(),
                               [bit](auto& item) { return bool(item & (1 << bit)) == true; });
    return {trues, input.data.size() - trues};
}

std::pair<uint16_t, uint16_t> decode_p1(const Input& input) {
    uint16_t gamma = 0;
    for (size_t i = 0; i < input.n_bits; i++) {
        const auto [t, f] = count_bit(input, i);
        if (t > f) {
            gamma |= 1 << i;
        }
    }
    uint16_t epsilon = ~(gamma | (0xFFFF << input.n_bits));
    return {gamma, epsilon};
}

uint16_t decode_p2(const Input& input, bool search_most_common) {
    auto candidates(input);
    int32_t bit = input.n_bits - 1;
    while (bit >= 0) {
        const auto [t, f] = count_bit(candidates, bit);
        bool search = t >= f != search_most_common;
        candidates.data.erase(std::remove_if(candidates.data.begin(), candidates.data.end(),
                                             [bit, search](auto& item) {
                                                 return bool(item & (1 << bit)) == search;
                                             }),
                              candidates.data.end());

        if (candidates.data.size() == 1) {
            return candidates.data.front();
        }
        bit--;
    }
    return 0;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input input;
    std::string line;
    while (std::getline(line_stream, line)) {
        if (input.data.empty()) {
            input.n_bits = line.size();
        }
        input.data.push_back(parse_line(line));
    }
    return input;
}

Result Day::partOne(const Input& input) const {
    const auto [gamma, epsilon] = decode_p1(input);
    return gamma * epsilon;
}

Result Day::partTwo(const Input& input) const {
    const auto oxygen_generator_rating = decode_p2(input, true);
    const auto co2_scrubber_rating = decode_p2(input, false);

    return oxygen_generator_rating * co2_scrubber_rating;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc