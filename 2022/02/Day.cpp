#include "Day.h"

#include "FactoryManager.h"

#include <map>

namespace aoc {

namespace {

enum class RPS {
    Rock,
    Paper,
    Scissors,
};

enum class Round {
    Win,
    Lose,
    Draw,
};

const std::map<char, RPS> opponentToRPS{
    {'A', RPS::Rock    },
    {'B', RPS::Paper   },
    {'C', RPS::Scissors},
};

const std::map<char, RPS> handToRPS{
    {'X', RPS::Rock    },
    {'Y', RPS::Paper   },
    {'Z', RPS::Scissors},
};

const std::map<char, Round> handToRound{
    {'X', Round::Lose},
    {'Y', Round::Draw},
    {'Z', Round::Win },
};

const std::map<Round, int32_t> roundScore{
    {Round::Lose, 0},
    {Round::Draw, 3},
    {Round::Win,  6},
};

const std::map<RPS, int32_t> rpsScore{
    {RPS::Rock,     1},
    {RPS::Paper,    2},
    {RPS::Scissors, 3},
};

const std::map<std::pair<RPS, RPS>, Round> evaluate{
    {{RPS::Rock, RPS::Rock},         Round::Draw},
    {{RPS::Rock, RPS::Paper},        Round::Win },
    {{RPS::Rock, RPS::Scissors},     Round::Lose},
    {{RPS::Paper, RPS::Rock},        Round::Lose},
    {{RPS::Paper, RPS::Paper},       Round::Draw},
    {{RPS::Paper, RPS::Scissors},    Round::Win },
    {{RPS::Scissors, RPS::Rock},     Round::Win },
    {{RPS::Scissors, RPS::Paper},    Round::Lose},
    {{RPS::Scissors, RPS::Scissors}, Round::Draw},
};

const std::map<std::pair<RPS, Round>, RPS> select{
    {{RPS::Rock, Round::Draw},     RPS::Rock    },
    {{RPS::Rock, Round::Win},      RPS::Paper   },
    {{RPS::Rock, Round::Lose},     RPS::Scissors},
    {{RPS::Paper, Round::Lose},    RPS::Rock    },
    {{RPS::Paper, Round::Draw},    RPS::Paper   },
    {{RPS::Paper, Round::Win},     RPS::Scissors},
    {{RPS::Scissors, Round::Win},  RPS::Rock    },
    {{RPS::Scissors, Round::Lose}, RPS::Paper   },
    {{RPS::Scissors, Round::Draw}, RPS::Scissors},
};

Result score(RPS opponent, RPS hand) {
    return roundScore.at(evaluate.at({opponent, hand})) + rpsScore.at(hand);
}

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
        result += score(opponentToRPS.at(opponent), handToRPS.at(hand));
    }
    return result;
}

Result Day::partTwo(const Input& input) const {
    Result result = 0;
    for (const auto& [opponent, hand] : input) {
        RPS selected = select.at({opponentToRPS.at(opponent), handToRound.at(hand)});
        result += score(opponentToRPS.at(opponent), selected);
    }
    return result;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc