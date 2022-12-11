#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <set>

namespace aoc {

class MonkeyBusiness {
  public:
    MonkeyBusiness(const std::vector<Monkey>& monkeys)
          : m_monkeys(monkeys) {
        std::set<size_t> divisors;
        for (const auto& monkey : m_monkeys) {
            divisors.insert(monkey.divisor);
        }
        m_denominator = 1;
        for (const auto& divisor : divisors) {
            m_denominator *= divisor;
        }
    }

  public:
    void round(bool denominate = false) {
        for (auto& monkey : m_monkeys) {
            for (auto& item : monkey.worry_levels) {
                item = monkey.operation(item);
                monkey.n_inspections++;
                if (denominate) {
                    item %= m_denominator;  // Part 2
                } else {
                    item /= 3;  // Part 1
                }
                size_t throw_to = monkey.test(item);
                m_monkeys.at(throw_to).worry_levels.push_back(item);
            }
            monkey.worry_levels.clear();
        }
    }

    size_t level() const {
        std::vector<size_t> inspections;
        for (const auto& monkey : m_monkeys) {
            inspections.push_back(monkey.n_inspections);
        }
        std::sort(inspections.begin(), inspections.end());
        std::reverse(inspections.begin(), inspections.end());
        return inspections.at(0) * inspections.at(1);
    }

  private:
    std::vector<Monkey> m_monkeys;
    size_t m_denominator;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    Monkey monkey;
    while (std::getline(line_stream, line)) {
        if (line.empty()) {
            inputs.push_back(monkey);
        } else {
            const auto [key, value] = helpers::trim(helpers::split(line, ": "));
            if (helpers::split(key, ' ').first == "Monkey") {
                monkey = Monkey();
            } else if (key == "Starting items") {
                for (const auto& item : helpers::tokenize(value, ", ")) {
                    monkey.worry_levels.push_back(std::stoul(item));
                }
            } else if (key == "Operation") {
                const auto args = helpers::tokenize(value, ' ');
                if (args.back() == "old") {
                    monkey.operation = [](size_t old) {
                        return old * old;
                    };
                } else if (args.at(3) == "+") {
                    int x = std::stoi(args.back());
                    monkey.operation = [x](size_t old) {
                        return old + x;
                    };
                } else {
                    int x = std::stoi(args.back());
                    monkey.operation = [x](size_t old) {
                        return old * x;
                    };
                }
            } else if (key == "Test") {
                auto tokens = helpers::tokenize(value, ' ');
                const size_t divisible_by = std::stoul(tokens.back());

                std::getline(line_stream, line);
                tokens = helpers::tokenize(line, ' ');
                const size_t when_true = std::stoul(tokens.back());

                std::getline(line_stream, line);
                tokens = helpers::tokenize(line, ' ');
                const size_t when_false = std::stoul(tokens.back());

                monkey.divisor = divisible_by;
                monkey.test = [divisible_by, when_true, when_false](size_t value) {
                    return (value % divisible_by) == 0 ? when_true : when_false;
                };
            }
        }
    }
    inputs.push_back(monkey);
    return inputs;
}

Result Day::partOne(const Input& input) const {
    MonkeyBusiness monkey_business(input);
    for (int i = 0; i < 20; i++) {
        monkey_business.round();
    }
    return monkey_business.level();
}

Result Day::partTwo(const Input& input) const {
    MonkeyBusiness monkey_business(input);
    for (int i = 0; i < 10000; i++) {
        monkey_business.round(true);
    }
    return monkey_business.level();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc