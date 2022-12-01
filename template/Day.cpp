#include "Day.h"

#include "FactoryManager.h"

namespace aoc {

namespace {

//

}  // namespace

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        //
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    //
}

Result Day::partTwo(const Input& input) const {
    //
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc