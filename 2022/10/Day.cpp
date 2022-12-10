#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <deque>
#include <iostream>

namespace aoc {

class Decoder {
    static constexpr char LIGHT{'#'};
    static constexpr char DARK{'.'};

  public:
    Decoder() = default;

  public:
    void execute(const Instruction& instruction) {
        cycle(instruction);
        if (instruction.addx) {
            // addx takes two cycles
            cycle({false});
        }
    }

    int32_t signalStrength() const {
        return m_signal_strength;
    }

    std::string screen() const {
        return m_pixels;
    }

  private:
    void cycle(const Instruction& instruction) {
        // Cycle start
        int32_t cycle_add{0};
        if (!m_stack.empty()) {
            cycle_add = m_stack.front();
            m_stack.pop_front();
        }
        if (instruction.addx) {
            m_stack.push_back(instruction.value);
        }

        // During cycle
        int32_t sprite_index = m_x - (m_cycle_counter % 40);
        if (-1 <= sprite_index && sprite_index <= 1) {
            m_pixels.push_back(LIGHT);
        } else {
            m_pixels.push_back(DARK);
        }

        // Cycle end
        m_cycle_counter++;
        if (m_cycle_counter == 20
            || m_cycle_counter > 20 && (m_cycle_counter - 20) % 40 == 0) {
            m_signal_strength += m_cycle_counter * m_x;
        }
        if (m_cycle_counter % 40 == 0) {
            m_pixels.push_back('\n');
        }
        if (cycle_add) {
            m_x += cycle_add;
        }
    }

  private:
    size_t m_cycle_counter{0};
    int32_t m_signal_strength{0};
    int32_t m_x{1};
    std::deque<int32_t> m_stack;
    std::string m_pixels;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        auto tokens = helpers::split(line, ' ');
        if (tokens.first == "addx") {
            inputs.push_back({true, std::stoi(tokens.second)});
        } else {
            inputs.push_back({false});
        }
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Decoder decoder;
    for (const auto& instruction : input) {
        decoder.execute(instruction);
    }
    return std::to_string(decoder.signalStrength());
}

Result Day::partTwo(const Input& input) const {
    Decoder decoder;
    for (const auto& instruction : input) {
        decoder.execute(instruction);
    }
    return decoder.screen();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc