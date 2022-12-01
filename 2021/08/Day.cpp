#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>

namespace aoc {

namespace {

enum class Digit : int8_t {
    One = 1,
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Zero = 0
};

static const std::map<std::string, int16_t> display_map_{
    {"abcefg",  0},
    {"cf",      1},
    {"acdeg",   2},
    {"acdfg",   3},
    {"bcdf",    4},
    {"abdfg",   5},
    {"abdefg",  6},
    {"acf",     7},
    {"abcdefg", 8},
    {"abcdfg",  9}
};

std::string difference(const std::string& X, const std::string& Y) {
    std::string diff;
    std::set_difference(X.begin(), X.end(), Y.begin(), Y.end(), std::back_inserter(diff));
    return diff;
}

std::string intersection(const std::string& X, const std::string& Y) {
    std::string inter;
    std::set_intersection(X.begin(), X.end(), Y.begin(), Y.end(), std::back_inserter(inter));
    return inter;
}

std::map<char, char> decode_inputs(const Signal& signal) {
    std::map<Digit, std::string> digits;
    std::map<size_t, std::vector<std::string>> unsorted;
    std::map<char, char> wiring;

    for (auto code : signal.in) {
        std::sort(code.begin(), code.end());
        switch (code.size()) {
            case 2:
                digits[Digit::One] = code;
                break;
            case 3:
                digits[Digit::Seven] = code;
                break;
            case 4:
                digits[Digit::Four] = code;
                break;
            case 7:
                digits[Digit::Eight] = code;
                break;
            default:
                unsorted[code.size()].push_back(code);
                break;
        }
    }

    {  // Find Seg::A - difference between 1 and 7
        auto diff = difference(digits[Digit::Seven], digits[Digit::One]);
        wiring[diff.front()] = 'a';
    }
    {  // Find Seg::C and Seg::F - compare 1 and six-segment unknowns (0,6,9)
        for (auto it = unsorted[6].begin(); it != unsorted[6].end(); ++it) {
            auto inter = intersection(digits[Digit::One], *it);
            if (inter.size() == 1) {
                auto diff = difference(digits[Digit::One], inter);
                wiring[diff.front()] = 'c';
                wiring[inter.front()] = 'f';
                digits[Digit::Six] = *it;
                unsorted[6].erase(it);
                break;
            }
        }
    }
    {  // Find Seg::D and Seg::B - compare 4 and six-segment unknowns (0,9)
        for (auto it = unsorted[6].begin(); it != unsorted[6].end(); ++it) {
            auto inter = intersection(digits[Digit::Four], *it);
            if (inter.size() == 3) {
                auto diff_four = difference(digits[Digit::Four], inter);
                wiring[diff_four.front()] = 'd';
                auto diff_one = difference(inter, digits[Digit::One]);
                wiring[diff_one.front()] = 'b';
                digits[Digit::Zero] = *it;
                unsorted[6].erase(it);
                digits[Digit::Nine] = unsorted[6].front();
                break;
            }
        }
    }
    {  // Find Seg::E - compare 9 and 8
        auto diff = difference(digits[Digit::Eight], digits[Digit::Nine]);
        wiring[diff.front()] = 'e';
    }
    {  // Find Seg:G - only one left
        for (const auto& segment : digits[Digit::Eight]) {
            if (wiring.count(segment) == 0) {
                wiring[segment] = 'g';
                break;
            }
        }
    }
    return wiring;
}

std::string remap(const std::string& code, std::map<char, char> wiring) {
    std::string decoded;
    std::transform(code.begin(), code.end(), std::back_inserter(decoded),
                   [wiring](char c) { return wiring.at(c); });
    std::sort(decoded.begin(), decoded.end());
    return decoded;
}

int16_t decode_outputs(const Signal& signal, std::map<char, char> wiring) {
    int16_t display = 0;
    for (size_t i = 0; i < signal.out.size(); i++) {
        auto x = remap(signal.out[i], wiring);
        display += display_map_.at(x) * pow(10, signal.out.size() - i - 1);
    }
    return display;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line, code;
    while (std::getline(line_stream, line)) {
        Signal sig;
        std::stringstream ss(line);
        for (size_t i = 0; i < sig.in.size(); i++) {
            std::getline(ss, code, ' ');
            sig.in[i] = code;
        }
        std::getline(ss, code, ' ');
        for (size_t i = 0; i < sig.out.size(); i++) {
            std::getline(ss, code, ' ');
            sig.out[i] = code;
        }
        inputs.push_back(sig);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    size_t count = 0;
    for (const auto& signal : input) {
        for (const auto& code : signal.out) {
            switch (code.size()) {
                case 2:  // one
                case 3:  // seven
                case 4:  // four
                case 7:  // eight
                    count++;
                    break;
                default:
                    break;
            }
        }
    }
    return count;
}

Result Day::partTwo(const Input& input) const {
    size_t sum = 0;
    for (const auto& signal : input) {
        auto wiring = decode_inputs(signal);
        sum += decode_outputs(signal, wiring);
    }
    return sum;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc