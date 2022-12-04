#pragma once

#include <string>
#include <vector>

namespace aoc::helpers {

std::vector<std::string> tokenize(const std::string& line, char delimiter) {
    size_t start = 0;
    size_t end;
    std::vector<std::string> tokens;

    while ((end = line.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(line.substr(start));

    return tokens;
}

}  // namespace aoc::helpers