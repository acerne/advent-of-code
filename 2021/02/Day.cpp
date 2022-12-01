#include "Day.h"

#include "FactoryManager.h"

#include <map>
#include <numeric>
#include <sstream>

namespace aoc {

namespace {

static const std::map<std::string, Command> string_to_command_{
    {"forward", Command::FORWARD},
    {"up",      Command::UP     },
    {"down",    Command::DOWN   }
};

struct Submarine1 {
    int32_t m_horizontal;
    int32_t m_depth;

    void move(Command command, int32_t value) {
        switch (command) {
            case Command::FORWARD:
                m_horizontal += value;
                break;
            case Command::UP:
                m_depth -= value;
                break;
            case Command::DOWN:
                m_depth += value;
                break;
        }
    }
};

struct Submarine2 {
    int32_t m_horizontal;
    int32_t m_depth;
    int32_t m_aim;

    void move(Command command, int32_t value) {
        switch (command) {
            case Command::FORWARD:
                m_horizontal += value;
                m_depth += value * m_aim;
                break;
            case Command::UP:
                m_aim -= value;
                break;
            case Command::DOWN:
                m_aim += value;
                break;
        }
    }
};

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line, command, value;
    while (std::getline(line_stream, line)) {
        std::stringstream ss(line);
        getline(ss, command, ' ');
        getline(ss, value, ' ');
        inputs.emplace_back(string_to_command_.at(command), std::stoi(value));
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Submarine1 sub{0, 0};
    for (const auto& [command, value] : input) {
        sub.move(command, value);
    }
    return sub.m_horizontal * sub.m_depth;
}

Result Day::partTwo(const Input& input) const {
    Submarine2 sub{0, 0, 0};
    for (const auto& [command, value] : input) {
        sub.move(command, value);
    }
    return sub.m_horizontal * sub.m_depth;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc