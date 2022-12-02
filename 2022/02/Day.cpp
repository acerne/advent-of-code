#include "Day.h"

#include "FactoryManager.h"

#include <map>

namespace aoc {

namespace {

enum Round : int32_t {
    Lose = 0,
    Draw = 3,
    Win = 6,
};

const std::map<char, int32_t> handScore{
    {'X', 1},
    {'Y', 2},
    {'Z', 3},
};

const std::map<char, int32_t> roundScore{
    {'X', Round::Lose},
    {'Y', Round::Draw},
    {'Z', Round::Win },
};

const std::map<std::pair<char, char>, int32_t> evaluate{
    {{'A', 'X'}, Round::Draw},
    {{'A', 'Y'}, Round::Win },
    {{'A', 'Z'}, Round::Lose},
    {{'B', 'X'}, Round::Lose},
    {{'B', 'Y'}, Round::Draw},
    {{'B', 'Z'}, Round::Win },
    {{'C', 'X'}, Round::Win },
    {{'C', 'Y'}, Round::Lose},
    {{'C', 'Z'}, Round::Draw},
};

const std::map<std::pair<char, char>, char> convert{
    {{'A', 'X'}, 'Z'},
    {{'A', 'Y'}, 'X'},
    {{'A', 'Z'}, 'Y'},
    {{'B', 'X'}, 'X'},
    {{'B', 'Y'}, 'Y'},
    {{'B', 'Z'}, 'Z'},
    {{'C', 'X'}, 'Y'},
    {{'C', 'Y'}, 'Z'},
    {{'C', 'Z'}, 'X'},
};

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        inputs.emplace_back(line.at(0), line.at(2));
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Result result = 0;
    for (const auto& [opponent, hand] : input) {
        result += evaluate.at({opponent, hand}) + handScore.at(hand);
    }
    return result;
}

Result Day::partTwo(const Input& input) const {
    Result result = 0;
    for (const auto& [opponent, round] : input) {
        char hand = convert.at({opponent, round});
        result += roundScore.at(round) + handScore.at(hand);
    }
    return result;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc