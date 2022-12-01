#include "Day.h"

#include "FactoryManager.h"

#include <deque>
#include <iostream>

namespace aoc {

namespace {

struct Coord {
    int x;
    int y;
};

class Octopi {
  public:
    Octopi(Input state)
          : m_state(std::move(state)),
            m_rows(m_state.size()),
            m_cols(m_state.front().size()),
            m_flash_counter(0) {}

    size_t flash() {
        size_t n_flashes = 0;
        // increment
        for (int y = 0; y < m_rows; y++) {
            for (int x = 0; x < m_cols; x++) {
                if (++m_state[y][x] == 10) {
                    m_flash_queued.push_back(Coord{.x = x, .y = y});
                }
            }
        }
        // flash
        while (!m_flash_queued.empty()) {
            auto next = m_flash_queued.front();
            flash_octopus(next.x, next.y);
            m_flash_queued.pop_front();
            n_flashes++;
        }
        // reset
        for (int y = 0; y < m_rows; y++) {
            for (int x = 0; x < m_cols; x++) {
                if (m_state[y][x] > 9) {
                    m_state[y][x] = 0;
                }
            }
        }
        m_flash_counter += n_flashes;
        return n_flashes;
    }

    void draw() const {
        for (int y = 0; y < m_rows; y++) {
            for (int x = 0; x < m_cols; x++) {
                std::cout << int(m_state[y][x]);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    size_t flash_count() {
        return m_flash_counter;
    }

  private:
    void flash_octopus(int x, int y) {
        for (int i = std::max(y - 1, 0); i <= std::min(y + 1, m_rows - 1); i++) {
            for (int j = std::max(x - 1, 0); j <= std::min(x + 1, m_cols - 1); j++) {
                if (!(i == y && j == x)) {
                    if (++m_state[i][j] == 10) {
                        m_flash_queued.push_back(Coord{.x = j, .y = i});
                    }
                }
            }
        }
    }

  private:
    Input m_state;
    std::deque<Coord> m_flash_queued;
    const int m_rows;
    const int m_cols;
    size_t m_flash_counter;
};

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    size_t y = 0;
    while (std::getline(line_stream, line)) {
        for (size_t x = 0; x < line.size(); x++) {
            inputs[y][x] = int8_t(line.at(x) - '0');
        }
        y++;
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Octopi octopi(input);

    for (int i = 0; i < 100; i++) {
        // octopi.draw();
        octopi.flash();
    }

    return octopi.flash_count();
}

Result Day::partTwo(const Input& input) const {
    Octopi octopi(input);

    size_t n_flashes = 0;
    size_t step = 0;
    do {
        n_flashes = octopi.flash();
        step++;
    } while (n_flashes != 100);

    return step;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc