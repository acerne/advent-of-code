#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>
#include <set>

namespace aoc {

struct Player {
    int32_t place;
    int32_t score{0};

    bool operator==(const Player& rhs) const {
        return std::tie(place, score) == std::tie(rhs.place, rhs.score);
    }

    bool operator<(const Player& rhs) const {
        return std::tie(place, score) < std::tie(rhs.place, rhs.score);
    }
};

struct Die {
    int32_t next{1};
    size_t rolls{0};

    int32_t roll() {
        int32_t current = next;
        if (++next > 100) {
            next = 1;
        }
        rolls++;
        return current;
    }
};

class Board {
  public:
    Board(Player player1, Player player2)
          : m_player1(player1),
            m_player2(player2),
            m_turn(true) {}

    void turn() {
        int32_t steps{0};
        for (int i = 0; i < 3; i++) {
            steps += m_die.roll();
        }
        if (m_turn) {
            move(m_player1, steps);
        } else {
            move(m_player2, steps);
        }
        m_turn = !m_turn;
    }

    bool check() {
        return m_player1.score >= 1000 || m_player2.score >= 1000;
    }

    int32_t evaluate() {
        return std::min(m_player1.score, m_player2.score) * m_die.rolls;
    }

    void print() {
        std::cout << "Player 1: " << m_player1.place << " / " << m_player1.score << std::endl;
        std::cout << "Player 2: " << m_player2.place << " / " << m_player2.score << std::endl;
        std::cout << "Die: " << m_die.next << " / " << m_die.rolls << std::endl;
    }

    static void move(Player& player, int32_t steps) {
        player.place = (player.place + steps - 1) % 10 + 1;
        player.score += player.place;
    }

  private:
    bool m_turn;
    Die m_die;
    Player m_player1;
    Player m_player2;
};

struct QuantumState {
    bool turn;
    Player player1;
    Player player2;

    bool operator==(const QuantumState& rhs) const {
        return std::tie(turn, player1, player2)
               == std::tie(rhs.turn, rhs.player1, rhs.player2);
    }

    bool operator<(const QuantumState& rhs) const {
        return std::tie(turn, player1, player2) < std::tie(rhs.turn, rhs.player1, rhs.player2);
    }

    void advance(int32_t roll) {
        if (turn) {
            Board::move(player1, roll);
        } else {
            Board::move(player2, roll);
        }
        turn = !turn;
    }
};

struct QuantumWins {
    size_t player1{0};
    size_t player2{0};

    QuantumWins& operator+=(const QuantumWins& rhs) {
        player1 += rhs.player1;
        player2 += rhs.player2;
        return *this;
    }

    friend QuantumWins operator*(const QuantumWins& lhs, size_t rhs) {
        QuantumWins out(lhs);
        out.player1 *= rhs;
        out.player2 *= rhs;
        return out;
    }
};

class QuantumBoard {
    static constexpr int32_t WIN_CONDITION{21};

  public:
    QuantumBoard(Player player1, Player player2)
          : m_initial_state(
              QuantumState{.turn = true, .player1 = player1, .player2 = player2}) {}

    QuantumWins recursive_search(const QuantumState& state) {
        static const std::map<int32_t, int32_t> POSSIBLE_ROLLS{
            {3, 1}, // 1 1 1
            {4, 3}, // 1 1 2 | 1 2 1 | 2 1 1
            {5, 6}, // 1 1 3 | 1 3 1 | 3 1 1 | 1 2 2 | 2 1 2 | 2 2 1
            {6, 7}, // 1 2 3 | 1 3 2 | 2 3 1 | 2 1 3 | 3 1 2 | 3 2 1 | 2 2 2
            {7, 6}, // 1 3 3 | 3 1 3 | 3 3 1 | 2 2 3 | 2 3 2 | 3 2 2
            {8, 3}, // 2 3 3 | 3 2 3 | 3 3 2
            {9, 1}, // 3 3 3
        };

        QuantumWins wins;

        for (const auto& [roll_sum, possibilities] : POSSIBLE_ROLLS) {
            for (int i = 0; i < possibilities; i++) {
                QuantumState next_state = state;
                next_state.advance(roll_sum);

                if (m_state_cache.contains(next_state)) {
                    wins += m_state_cache.at(next_state);
                } else {
                    QuantumWins next_wins;
                    if (next_state.player1.score >= WIN_CONDITION) {
                        // If player 1 won
                        next_wins.player1 += 1;
                    } else if (next_state.player2.score >= WIN_CONDITION) {
                        // If player 2 won
                        next_wins.player2 += 1;
                    } else {
                        // Else search further
                        next_wins = recursive_search(next_state);
                    }
                    m_state_cache[next_state] = next_wins;
                    wins += next_wins;
                }
            }
        }
        return wins;
    }

    QuantumWins run() {
        return recursive_search(m_initial_state);
    }

  private:
    QuantumState m_initial_state;
    std::map<QuantumState, QuantumWins> m_state_cache;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    std::getline(line_stream, line);
    inputs.first = std::stoi(helpers::tokenize(line, ' ').back());
    std::getline(line_stream, line);
    inputs.second = std::stoi(helpers::tokenize(line, ' ').back());
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Board board(Player{.place = input.first}, Player{.place = input.second});
    while (!board.check()) {
        board.turn();
        // board.print();
    }
    return board.evaluate();
}

Result Day::partTwo(const Input& input) const {
    QuantumBoard board(Player{.place = input.first}, Player{.place = input.second});

    const auto wins = board.run();

    return std::max(wins.player1, wins.player2);
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc