#include "Runner.h"

// Usage: AocRunner [<year> [<day> [<name>]]]
int main(int argc, char* argv[]) {
    aoc::Runner runner(argc, argv);

    runner.load();
    runner.run();

    return 0;
}