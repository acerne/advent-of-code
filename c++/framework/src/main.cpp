#include "datatypes.hpp"
#include "day.hpp"

#include <dlfcn.h>
#include <memory>

int main() {
  std::vector<IDay*> days;

  IDay* (*create)();
  void (*destroy)(IDay*);

  for (int day = 1; day < 4; day++) {
    const std::string day_lib("./lib" + Day::to_string_id(day) + ".so");
    void* handle = dlopen(day_lib.c_str(), RTLD_LAZY);

    create = (IDay * (*)()) dlsym(handle, "create_object");
    destroy = (void (*)(IDay*))dlsym(handle, "destroy_object");

    days.push_back((IDay*)create());
  }

  for (const auto& day : days) {
    day->load();
    day->run();
  }

  for (const auto day : days) {
    destroy(day);
  }
}