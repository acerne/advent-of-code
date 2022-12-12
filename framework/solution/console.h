#pragma once

#include <string>
#include <vector>

namespace aoc::console {

std::string red(const std::string& text) {
    return "\e[31m" + text + "\e[0m";
}

std::string green(const std::string& text) {
    return "\e[32m" + text + "\e[0m";
}

}  // namespace aoc::console