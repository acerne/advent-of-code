#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <set>

namespace aoc {

namespace {

size_t checkSequence(const std::string& sequence) {
    std::set<char> charset;
    for (const auto& c : sequence) {
        charset.insert(c);
    }
    return charset.size();
}

size_t search(const std::string& input, size_t window) {
    for (int i = 0; i < input.size(); i++) {
        if (checkSequence(input.substr(i, window)) == window) {
            return i + window;
        }
    }
    return -1;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::getline(line_stream, inputs);
    return inputs;
}

Result Day::partOne(const Input& input) const {
    return search(input, 4);
}

Result Day::partTwo(const Input& input) const {
    return search(input, 14);
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc