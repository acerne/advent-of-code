#include "Runner.h"

#include <iostream>

#include "Timer.h"

namespace aoc {

Runner::Runner(int argc, char* argv[]) {
    if (argc > 1) {
        m_name = argv[1];
    }
}

void Runner::load() {
    std::cout << "Loading solutions..." << std::endl;
    m_solutions.clear();
    Timer loading;
    aoc::FactoryManager manager;
    loading.start();
    std::filesystem::directory_iterator dir("./solutions/");
    for (const auto& item : dir) {
        if (item.is_regular_file() && item.path().extension() == ".so") {
            manager.loadFactories(item);
            auto solutions = manager.createAllSolutions();
            m_solutions.merge(solutions);
        }
    }
    loading.stop();
    std::cout << m_solutions.size() << " solutions loaded (" << loading.duration() << " ms)."
              << std::endl;
}

void Runner::run() {
    static std::map<Part, std::string> part_string{
        {Part::One, "1"},
        {Part::Two, "2"}
    };

    for (const auto& [name, solution] : m_solutions) {
        if (m_name && *m_name != name) {
            continue;
        }

        std::cout << std::endl << "Running solution " << name << std::endl;
        Timer loading;
        Timer calculation;

        for (const auto& part : {Part::One, Part::Two}) {
            std::cout << " Part " << part_string.at(part) << ":" << std::endl;
            loading.start();
            solution->load();
            loading.stop();
            if (solution->test(part)) {
                std::cout << "  Test passed." << std::endl;
            } else {
                std::cout << "  Test FAILED!" << std::endl;
                std::cout << "  - Expected result = " << solution->expected(part) << std::endl;
                std::cout << "  - Actual result   = " << solution->result(part) << std::endl;
            }
            calculation.start();
            solution->solve(part);
            calculation.stop();
            std::cout << "  Result   = " << solution->result(part) << std::endl;
            std::cout << "  - Loading took " << loading.duration() << " ms" << std::endl;
            std::cout << "  - Calculation took " << calculation.duration() << " ms"
                      << std::endl;
        }
    }
}

}  // namespace aoc