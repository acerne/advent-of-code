#pragma once

#include "interface.hpp"

#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

class Loader
{
public:
  Loader() = delete;

  static std::vector<std::shared_ptr<IDay>> load_all() {
    std::vector<int> days;
    const int last_day = 25;
    for (int day = 1; day <= last_day; day++) {
      days.push_back(day);
    }
    return load_some(days);
  }

  static std::vector<std::shared_ptr<IDay>> load_some(const std::vector<int>& days) {
    std::vector<std::shared_ptr<IDay>> libs;
    for (const auto day : days) {
      const std::string lib_file("./lib" + day_to_string_id(day) + ".so");
      if (file_exists(lib_file)) {
        std::cout << "Loading lib " << lib_file << std::endl;
        void* handle = dlopen(lib_file.c_str(), RTLD_LAZY);

        IDay* (*create)();
        create = (IDay * (*)()) dlsym(handle, "create_object");

        libs.emplace_back(std::shared_ptr<IDay>((IDay*)create(), [handle](IDay* ptr) {
          void (*destroy)(IDay*);
          destroy = (void (*)(IDay*))dlsym(handle, "destroy_object");
          destroy(ptr);
        }));
      }
    }
    return libs;
  }

private:
  static bool file_exists(const std::string& path) {
    std::ifstream ifile;
    ifile.open(path);
    if (ifile) {
      return true;
    }
    return false;
  }
};
