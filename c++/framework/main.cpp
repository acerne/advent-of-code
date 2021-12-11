#include "loader.hpp"

int main(int argc, char* argv[]) {

  std::vector<std::shared_ptr<IDay>> days;

  if (argc > 1) {
    std::vector<int> args;
    for (int i = 1; i < argc; i++) {
      args.push_back(std::stoi(argv[i]));
    }
    days = DayLoader::load_some(args);
  } else {
    days = DayLoader::load_all();
  }

  for (auto& day : days) {
    day.get()->execute();
  }
}