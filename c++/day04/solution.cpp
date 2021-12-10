

#include "datatypes.hpp"
#include "day.hpp"

#include <fstream>
#include <optional>
#include <sstream>

namespace
{
class Player
{
public:
  Player(Board board)
    : m_board(std::move(board))
    , m_hits_x{}
    , m_hits_y{} {}

  void hit(int8_t number) {
    for (size_t y = 0; y < 5; y++) {
      for (size_t x = 0; x < 5; x++) {
        if (m_board[x][y] == number) {
          m_hits_x[x]++;
          m_hits_y[y]++;
          m_board[x][y] = -1;
          return;
        }
      }
    }
  }

  std::optional<int32_t> check() const {
    for (int i = 0; i < 5; i++) {
      if (m_hits_x[i] == 5 || m_hits_y[i] == 5) {
        int32_t sum{};
        for (size_t y = 0; y < 5; y++) {
          for (size_t x = 0; x < 5; x++) {
            if (m_board[x][y] > 0) {
              sum += m_board[x][y];
            }
          }
        }
        return sum;
      }
    }
    return {};
  }

private:
  Board m_board;
  std::array<int8_t, 5> m_hits_x;
  std::array<int8_t, 5> m_hits_y;
};

} // namespace

Input Day::read(const std::string& file_path) {
  Input inputs;
  std::ifstream line_stream(file_path);
  std::string line, number;
  {
    std::getline(line_stream, line);
    std::stringstream ss(line);
    while (getline(ss, number, ',')) {
      inputs.numbers.push_back(std::stoi(number));
    }
  }
  while (std::getline(line_stream, line)) {
    if (line.empty()) {
      continue;
    }
    Board board;
    for (size_t y = 0; y < 5; y++) {
      for (size_t x = 0; x < 5; x++) {
        board[x][y] = std::stoi(line.substr(x * 3, 3));
      }
      std::getline(line_stream, line);
    }
    inputs.boards.push_back(board);
  }
  return inputs;
}

Result Day::Part1::process(const Input& input) {
  std::vector<Player> players;
  for (const auto& board : input.boards) {
    players.emplace_back(board);
  }
  for (const auto& draw : input.numbers) {
    for (auto& player : players) {
      player.hit(draw);
      auto bingo = player.check();
      if (bingo) {
        return *bingo * draw;
      }
    }
  }
  return -1;
}

Result Day::Part2::process(const Input& input) {
  std::vector<Player> players;
  for (const auto& board : input.boards) {
    players.emplace_back(board);
  }
  for (const auto& draw : input.numbers) {
    for (auto player_it = players.begin(); player_it != players.end();) {
      player_it->hit(draw);
      auto bingo = player_it->check();
      if (bingo) {
        if (players.size() > 1) {
          player_it = players.erase(player_it);
          continue;
        }
        return *bingo * draw;
      }
      ++player_it;
    }
  }
  return -1;
}

extern "C" Day* create_object(int num) {
  return new Day();
}

extern "C" void destroy_object(Day* object) {
  delete object;
}