#include "Day.h"

#include "FactoryManager.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>

namespace aoc {

namespace {

bool check_right(const Input& input, size_t x, size_t y) {
    if (x == input.front().size() - 1) {
        return true;
    }
    return input.at(y).at(x) < input.at(y).at(x + 1);
}

bool check_down(const Input& input, size_t x, size_t y) {
    if (y == input.size() - 1) {
        return true;
    }
    return input.at(y).at(x) < input.at(y + 1).at(x);
}

bool check_left(const Input& input, size_t x, size_t y) {
    if (x == 0) {
        return true;
    }
    return input.at(y).at(x) < input.at(y).at(x - 1);
}

bool check_up(const Input& input, size_t x, size_t y) {
    if (y == 0) {
        return true;
    }
    return input.at(y).at(x) < input.at(y - 1).at(x);
}

std::vector<int8_t> local_minima(const Input& input) {
    std::vector<int8_t> minima;

    size_t rows = input.size();
    size_t cols = input.front().size();

    for (size_t y = 0; y < rows; y++) {
        for (size_t x = 0; x < cols; x++) {
            if (check_right(input, x, y)) {
                if (check_down(input, x, y)) {
                    if (check_left(input, x, y)) {
                        if (check_up(input, x, y)) {
                            minima.push_back(input.at(y).at(x));
                        }
                    }
                }
            }
        }
    }
    return minima;
}

using Chord = std::pair<int, int>;
using Region = std::multimap<size_t, Chord>;

Region get_global_region(const Input& input) {
    Region region;

    size_t rows = input.size();
    size_t cols = input.front().size();

    for (size_t y = 0; y < rows; y++) {
        Chord c{0, -1};
        for (size_t x = 0; x < cols; x++) {
            if (input.at(y).at(x) == 9) {
                if (c.second >= 0) {
                    region.emplace(y, c);
                }
                c = {x + 1, -1};
            } else {
                c.second = x;
            }
        }
        if (c.second >= 0) {
            region.emplace(y, c);
        }
    }
    return region;
}

bool overlap(const Chord& a, const Chord& b) {
    if (a.first >= b.first && a.first <= b.second) {
        return true;
    }
    if (a.second >= b.first && a.second <= b.second) {
        return true;
    }
    if (b.first >= a.first && b.first <= a.second) {
        return true;
    }
    if (b.second >= a.first && b.second <= a.second) {
        return true;
    }
    return false;
}

std::vector<Region> split_regions(Region region) {
    std::vector<Region> regions;
    while (!region.empty()) {
        Region subregion;
        auto next = region.extract(region.begin()->first);
        subregion.insert(std::move(next));

        // search down
        for (auto it = subregion.begin(); it != subregion.end(); ++it) {
            auto range = region.equal_range(it->first + 1);
            for (auto range_it = range.first; range_it != range.second;) {
                if (overlap(it->second, range_it->second)) {
                    subregion.insert(*range_it);
                    range_it = region.erase(range_it);
                } else
                    range_it++;
            }
        }

        // search up
        for (auto it = subregion.rbegin(); it != subregion.rend(); ++it) {
            auto range = region.equal_range(it->first - 1);
            for (auto range_it = range.first; range_it != range.second;) {
                if (overlap(it->second, range_it->second)) {
                    subregion.insert(*range_it);
                    range_it = region.erase(range_it);
                } else
                    range_it++;
            }
        }
        regions.push_back(std::move(subregion));
    }
    return regions;
}

void draw(const std::vector<Region>& regions, const Input& input) {
    std::vector<std::vector<char>> display(input.size(),
                                           std::vector<char>(input.front().size(), ' '));
    std::string ids = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t i = 0;
    for (const auto& region : regions) {
        const char id = ids.at(i);
        for (const auto& chord : region) {
            const auto y = chord.first;
            for (size_t x = chord.second.first; x <= chord.second.second; x++) {
                display.at(y).at(x) = id;
            }
        }
        if (++i >= ids.size()) {
            i = 0;
        }
    }
    for (const auto& row : display) {
        for (const auto& c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

std::vector<size_t> area(const std::vector<Region>& regions) {
    std::vector<size_t> areas;
    for (const auto& region : regions) {
        size_t area = 0;
        for (const auto& chord : region) {
            area += (chord.second.second - chord.second.first) + 1;
        }
        areas.push_back(area);
    }
    return areas;
}

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        std::vector<int8_t> row;
        std::transform(line.begin(), line.end(), std::back_inserter(row),
                       [](auto& c) { return (c - '0'); });
        inputs.push_back(row);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    const auto minima = local_minima(input);
    auto sum = std::accumulate(minima.begin(), minima.end(), size_t(0));
    sum += minima.size();
    return sum;
}

Result Day::partTwo(const Input& input) const {
    const auto region = get_global_region(input);
    const auto regions = split_regions(region);
    // draw(regions, input);
    auto areas = area(regions);
    std::sort(areas.begin(), areas.end());
    const auto result = std::accumulate(areas.rbegin(), areas.rbegin() + 3, size_t(1),
                                        std::multiplies<size_t>());
    return result;
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc