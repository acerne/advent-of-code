#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>

namespace aoc {

struct Monkey {
    std::string name;
    virtual double getValue() const = 0;
    virtual bool contains(const std::string& monkey) const = 0;

    Monkey(const std::string& name)
          : name(name) {}
};

struct MonkeyNumber : public Monkey {
    int64_t value;

    MonkeyNumber(const std::string& name, double value)
          : Monkey(name),
            value(value) {}

    double getValue() const override {
        return static_cast<double>(value);
    }

    bool contains(const std::string& monkey) const override {
        return monkey == name;
    }
};

struct MonkeyOperation : public Monkey {
    enum class Type {
        Plus,
        Minus,
        Mult,
        Div,
    } type;
    std::shared_ptr<Monkey> a;
    std::shared_ptr<Monkey> b;

    MonkeyOperation(const std::string& name, Type type)
          : Monkey(name),
            type(type) {}

    double getValue() const override {
        // Though values can fit int64, opreations should use double to accomodate the division
        // when guressing the part 2. Usint int64 may produce multiple wrong results due to
        // rounding.
        switch (type) {
            case Type::Plus: {
                return a->getValue() + b->getValue();
            }
            case Type::Minus: {
                return a->getValue() - b->getValue();
            }
            case Type::Mult: {
                return a->getValue() * b->getValue();
            }
            case Type::Div: {
                return a->getValue() / b->getValue();
            }
            default:
                throw std::runtime_error("Illegal state");
        }
    }

    bool contains(const std::string& monkey) const override {
        return a->contains(monkey) || b->contains(monkey);
    }
};

class Bisection {  // Part 2
  public:
    Bisection(const Input& input, size_t deadline = 10000)
          : m_input(input),
            m_deadline(deadline) {}

  public:
    int64_t find(int64_t low, int64_t high) {
        // Initial state
        m_human = std::dynamic_pointer_cast<MonkeyNumber>(m_input.at("humn"));
        m_root = std::dynamic_pointer_cast<MonkeyOperation>(m_input.at("root"));
        m_root_path = m_root->a->contains("humn");
        m_root_a = m_root->a->getValue();
        m_root_b = m_root->b->getValue();
        // Search until found or deadline of iterations is reached
        for (size_t n = 0; n < m_deadline; n++) {
            int64_t mid = (low + high) / 2;
            double root_diff = f(mid);
            if (root_diff == 0) {
                // Solution found
                return mid;
            }
            if (root_diff > 0 == f(low) > 0) {
                low = mid;
            } else {
                high = mid;
            }
        }
        return -1;
    }

  private:
    double f(int64_t value) const {
        m_human->value = value;
        if (m_root_path) {
            return m_root_b - m_root->a->getValue();
        } else {
            return m_root_a - m_root->b->getValue();
        }
    }

  private:
    const size_t m_deadline;
    Input m_input;
    std::shared_ptr<MonkeyNumber> m_human;
    std::shared_ptr<const MonkeyOperation> m_root;
    bool m_root_path;
    double m_root_a;
    double m_root_b;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    std::map<std::string, std::pair<std::string, std::string>> relations;
    while (std::getline(line_stream, line)) {
        const auto [name, job] = helpers::split(line, ": ");
        const auto job_details = helpers::tokenize(job, ' ');
        if (job_details.size() == 1) {
            inputs[name] = std::make_shared<MonkeyNumber>(name, std::stoi(job_details.at(0)));
        } else {
            static const std::map<std::string, MonkeyOperation::Type> operation{
                {"+", MonkeyOperation::Type::Plus },
                {"-", MonkeyOperation::Type::Minus},
                {"*", MonkeyOperation::Type::Mult },
                {"/", MonkeyOperation::Type::Div  },
            };
            inputs[name] =
                std::make_shared<MonkeyOperation>(name, operation.at(job_details.at(1)));
            relations[name] = {job_details.at(0), job_details.at(2)};
        }
    }
    for (const auto& [monkey, from] : relations) {
        auto op = std::dynamic_pointer_cast<MonkeyOperation>(inputs[monkey]);
        op->a = inputs.at(from.first);
        op->b = inputs.at(from.second);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    return input.at("root")->getValue();
}

Result Day::partTwo(const Input& input) const {
    Bisection bisection(input);
    int64_t result = bisection.find(std::numeric_limits<int64_t>::min() / 10,
                                    std::numeric_limits<int64_t>::max() / 10);
    return result;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc