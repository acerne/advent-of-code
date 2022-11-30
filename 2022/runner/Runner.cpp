#include "Runner.h"

#include <iostream>

#include "Timer.h"

namespace aoc {

Runner::Runner(int argc, char* argv[]) {
    if (argc > 1) {
        m_year = std::stoi(argv[1]);
    }
    if (argc > 2) {
        m_day = std::stoi(argv[2]);
    }
    if (argc > 3) {
        m_name = argv[3];
    }
}

void Runner::load() {
    std::cout << "Loading solutions..." << std::endl;
    Timer loading;
    aoc::FactoryManager manager;
    loading.start();
    manager.loadFactories("./libAocSolutions.so");
    m_solutions = manager.createAllSolutions();
    loading.stop();
    std::cout << m_solutions.size() << " solutions loaded (" << loading.duration() << " ms)."
              << std::endl;
}

void Runner::run() {
    for (const auto& [name, solution] : m_solutions) {
        if (m_year && *m_year != solution->year()) {
            continue;
        }
        if (m_day && *m_day != solution->day()) {
            continue;
        }
        if (m_name && *m_name != name) {
            continue;
        }

        std::cout << std::endl
                  << "Running solution " << name << " [" << solution->year() << "/"
                  << solution->day() << "]" << std::endl;
        Timer loading;
        Timer calculation;

        std::cout << " Part one:" << std::endl;
        solution->loadSample();
        solution->solvePartOne();
        if (solution->checkPartOne()) {
            std::cout << "  Test passed." << std::endl;
        } else {
            std::cout << "  Test FAILED!" << std::endl;
            std::cout << "  - Expected result = " << solution->expectedPartOne() << std::endl;
            std::cout << "  - Actual result   = " << solution->result() << std::endl;
        }
        loading.start();
        solution->loadInput();
        loading.stop();
        calculation.start();
        solution->solvePartOne();
        calculation.stop();
        std::cout << "  Result   = " << solution->result() << std::endl;
        std::cout << "  - Loading took " << loading.duration() << " ms" << std::endl;
        std::cout << "  - Calculation took " << calculation.duration() << " ms" << std::endl;

        std::cout << " Part two:" << std::endl;
        solution->loadSample();
        solution->solvePartTwo();
        if (solution->checkPartTwo()) {
            std::cout << "  Test passed." << std::endl;
        } else {
            std::cout << "  Test FAILED!" << std::endl;
            std::cout << "  - Expected result = " << solution->expectedPartTwo() << std::endl;
            std::cout << "  - Actual result   = " << solution->result() << std::endl;
        }
        loading.start();
        solution->loadInput();
        loading.stop();
        calculation.start();
        solution->solvePartTwo();
        calculation.stop();
        std::cout << "  Result   = " << solution->result() << std::endl;
        std::cout << "  - Loading took " << loading.duration() << " ms" << std::endl;
        std::cout << "  - Calculation took " << calculation.duration() << " ms" << std::endl;
    }
}

}  // namespace aoc