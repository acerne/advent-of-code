#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <set>

namespace aoc {

class Heightmap {
  public:
    Heightmap(const Input& input)
          : m_map(input),
            m_height(input.size()),
            m_width(input.front().size()) {}

  public:
    size_t countVisibleTrees() {
        m_visible_trees.clear();
        for (int y = 0; y < m_height; y++) {
            searchRow(y, false);
            searchRow(y, true);
        }
        for (int x = 0; x < m_width; x++) {
            searchColumn(x, false);
            searchColumn(x, true);
        }
        return m_visible_trees.size();
    }

    size_t bestScenicView() {
        size_t best_score{0};
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                size_t score = scoreRow(x, y, false);
                score *= scoreRow(x, y, true);
                score *= scoreColumn(x, y, false);
                score *= scoreColumn(x, y, true);
                if (score > best_score) {
                    best_score = score;
                }
            }
        }
        return best_score;
    }

  private:
    uint8_t getPixel(int x, int y) const {
        return m_map.at(y).at(x);
    }

    void searchRow(int y, bool reverse) {
        int first = reverse ? m_width - 1 : 0;
        uint8_t highest_tree = getPixel(first, y);
        m_visible_trees.insert({first, y});
        for (int i = 1; i < m_width; i++) {
            int x = reverse ? m_width - 1 - i : i;
            uint8_t h = getPixel(x, y);
            if (h > highest_tree) {
                highest_tree = h;
                m_visible_trees.insert({x, y});
            }
        }
    }

    void searchColumn(int x, bool reverse) {
        int first = reverse ? m_height - 1 : 0;
        uint8_t highest_tree = getPixel(x, first);
        m_visible_trees.insert({x, first});
        for (int i = 1; i < m_height; i++) {
            int y = reverse ? m_height - 1 - i : i;
            uint8_t h = getPixel(x, y);
            if (h > highest_tree) {
                highest_tree = h;
                m_visible_trees.insert({x, y});
            }
        }
    }

    size_t scoreRow(int x, int y, bool reverse) {
        size_t visible_trees = 0;
        uint8_t h1 = getPixel(x, y);
        int i = 1;
        int sign = reverse ? -1 : 1;
        int ix = x + sign * i;
        while (ix >= 0 && ix < m_width) {
            uint8_t h2 = getPixel(ix, y);
            if (h2 >= h1) {
                visible_trees++;
                break;
            }
            visible_trees++;
            ix = x + sign * ++i;
        }
        return visible_trees;
    }

    size_t scoreColumn(int x, int y, bool reverse) {
        size_t visible_trees = 0;
        uint8_t h1 = getPixel(x, y);
        int i = 1;
        int sign = reverse ? -1 : 1;
        int iy = y + sign * i;
        while (iy >= 0 && iy < m_height) {
            uint8_t h2 = getPixel(x, iy);
            if (h2 >= h1) {
                visible_trees++;
                break;
            }
            visible_trees++;
            iy = y + sign * ++i;
        }
        return visible_trees;
    }

  private:
    std::set<std::pair<int, int>> m_visible_trees;
    std::vector<std::vector<uint8_t>> m_map;
    size_t m_width;
    size_t m_height;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        std::vector<uint8_t> row;
        for (const auto& c : line) {
            row.push_back(static_cast<uint8_t>(c - '0'));
        }
        inputs.push_back(std::move(row));
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    Heightmap map(input);
    size_t visible_trees = map.countVisibleTrees();
    return visible_trees;
}

Result Day::partTwo(const Input& input) const {
    Heightmap map(input);
    size_t best_score = map.bestScenicView();
    return best_score;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc