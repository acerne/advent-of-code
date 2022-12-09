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

std::pair<std::string, std::string> split(const std::string& line, char delimiter) {
    std::pair<std::string, std::string> tokens;

    auto pos = line.find(delimiter, 0);
    if (pos != std::string::npos) {
        return {line.substr(0, pos), line.substr(pos + 1)};
    } else {
        return {line, ""};
    }
}

std::pair<std::string, std::string> split(const std::string& line,
                                          const std::string& delimiter) {
    std::pair<std::string, std::string> tokens;

    auto pos = line.find(delimiter, 0);
    if (pos != std::string::npos) {
        return {line.substr(0, pos), line.substr(pos + delimiter.size())};
    } else {
        return {line, ""};
    }
}

}  // namespace aoc::helpers