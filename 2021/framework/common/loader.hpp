#pragma once

#include "interfaces.hpp"

#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

class Loader
{
public:
  Loader() = delete;

protected:
  static bool file_exists(const std::string& path) {
    std::ifstream ifile;
    ifile.open(path);
    if (ifile) {
      return true;
    }
    return false;
  }

  template<class Interface>
  static std::shared_ptr<Interface> load(const std::string& file) {
    std::cout << "Loading lib " << file << std::endl;
    void* handle = dlopen(file.c_str(), RTLD_LAZY);

    std::string create_fn, destroy_fn;

    if constexpr (std::is_same<Interface, IDay>::value) {
      create_fn = "create_day";
      destroy_fn = "destroy_day";
    } else if constexpr (std::is_same<Interface, ISolution>::value) {
      create_fn = "create_solution";
      destroy_fn = "destroy_solution";
    }

    Interface* (*create)();
    create = (Interface * (*)()) dlsym(handle, create_fn.c_str());

    return std::shared_ptr<Interface>((Interface*)create(), [handle, destroy_fn](Interface* ptr) {
      void (*destroy)(Interface*);
      destroy = (void (*)(Interface*))dlsym(handle, destroy_fn.c_str());
      destroy(ptr);
    });
  }
};

class DayLoader : Loader
{
public:
  DayLoader() = delete;

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
        libs.emplace_back(load<IDay>(lib_file));
      }
    }
    return libs;
  }
};

class SolutionLoader : Loader
{
public:
  SolutionLoader() = delete;

  static std::vector<std::shared_ptr<ISolution>> load_all(int day) {
    std::vector<std::shared_ptr<ISolution>> libs;
    std::filesystem::directory_iterator dir("./" + day_to_string_id(day) + "/");
    for (const auto& item : dir) {
      if (item.is_regular_file() && item.path().extension() == ".so") {
        libs.emplace_back(load<ISolution>(item.path()));
      }
    }
    return libs;
  }
};
