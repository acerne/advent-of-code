#include "datatypes.hpp"
#include "day.hpp"

#include <dlfcn.h>
#include <memory>

int main() {
  std::vector<IDay*> days;

  IDay* (*create)(int);
  void (*destroy)(IDay*);

  for (int day = 1; day < 4; day++) {

    auto day_str = std::to_string(day);
    day_str.insert(0, 1, '0');

    const std::string day_lib("./libday" + day_str + ".so");
    void* handle = dlopen(day_lib.c_str(), RTLD_LAZY);

    create = (IDay * (*)(int)) dlsym(handle, "create_object");
    destroy = (void (*)(IDay*))dlsym(handle, "destroy_object");

    days.push_back((IDay*)create(day));
  }

  for (const auto& day : days) {
    day->load();
    day->run();
  }

  for (const auto day : days) {
    destroy(day);
  }
}