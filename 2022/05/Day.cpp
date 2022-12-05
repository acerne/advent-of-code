#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>

namespace aoc {

namespace {

class Crane {
  public:
    Crane(const Cargo& crates, const std::deque<Move>& moves)
          : m_crates(crates),
            m_moves(moves) {}

  public:
    virtual void move() = 0;

    size_t movesLeft() const {
        return m_moves.size();
    }

    std::string topmostCrates() const {
        std::string top;
        for (const auto& [idx, crates] : m_crates) {
            top.push_back(crates.back());
        }
        return top;
    }

    void print() const {
        std::string state;
        for (const auto& [idx, crates] : m_crates) {
            std::cout << idx << " ";
            for (const auto& crate : crates) {
                std::cout << crate << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

  protected:
    Cargo m_crates;
    std::deque<Move> m_moves;
};

class CrateMover9000 : public Crane {
  public:
    CrateMover9000(const Cargo& crates, const std::deque<Move>& moves)
          : Crane(crates, moves) {}

    // Move one crate at a time
    void move() override {
        Move next = m_moves.front();
        m_moves.pop_front();

        for (int i = 0; i < next.count; i++) {
            auto crate = m_crates[next.from].back();
            m_crates[next.from].pop_back();
            m_crates[next.to].push_back(crate);
        }
    };
};

class CrateMover9001 : public Crane {
  public:
    CrateMover9001(const Cargo& crates, const std::deque<Move>& moves)
          : Crane(crates, moves) {}

    // Move multiple crates at a time
    void move() override {
        Move next = m_moves.front();
        m_moves.pop_front();

        std::deque<char> crane;
        for (int i = 0; i < next.count; i++) {
            auto crate = m_crates[next.from].back();
            m_crates[next.from].pop_back();
            crane.push_back(crate);
        }
        for (int i = 0; i < next.count; i++) {
            auto crate = crane.back();
            crane.pop_back();
            m_crates[next.to].push_back(crate);
        }
    };
};

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    std::string line;

    // Parsing the cargo
    std::vector<std::string> cargo_strings;
    while (std::getline(line_stream, line)) {
        if (line.empty()) {
            break;
        }
        cargo_strings.push_back(line);
    }
    Cargo cargo;
    size_t pos{0};
    while ((pos = cargo_strings.back().find_first_not_of(' ', pos + 1)) != std::string::npos) {
        int idx = static_cast<int>(cargo_strings.back().at(pos) - 48);
        for (int i = cargo_strings.size() - 2; i >= 0; i--) {
            if (cargo_strings.at(i).at(pos) == ' ') {
                break;
            }
            cargo[idx].push_back(cargo_strings.at(i).at(pos));
        }
    }

    // Parsing the moves
    std::deque<Move> moves;
    while (std::getline(line_stream, line)) {
        auto tokens = helpers::tokenize(line, ' ');
        moves.push_back(Move{.count = std::stoi(tokens.at(1)),
                             .from = std::stoi(tokens.at(3)),
                             .to = std::stoi(tokens.at(5))});
    }

    return {cargo, moves};
}

Result Day::partOne(const Input& input) const {
    CrateMover9000 crane(input.first, input.second);

    while (crane.movesLeft() > 0) {
        crane.move();
        // crane.print();
    }

    return crane.topmostCrates();
}

Result Day::partTwo(const Input& input) const {
    CrateMover9001 crane(input.first, input.second);

    while (crane.movesLeft() > 0) {
        crane.move();
        // crane.print();
    }

    return crane.topmostCrates();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc