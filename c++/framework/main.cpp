#include "loader.hpp"

int main(int argc, char* argv[]) {

  std::vector<std::shared_ptr<IDay>> days;

  if (argc > 1) {
    std::vector<int> args;
    for (int i = 1; i < argc; i++) {
      args.push_back(std::stoi(argv[i]));
    }
    days = Loader::load_some(args);
  } else {
    days = Loader::load_all();
  }

  for (auto& day : days) {
    day.get()->load();
    day.get()->run();
  }
}