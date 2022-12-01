#include "FactoryManager.h"

#include <dlfcn.h>

namespace aoc {

FactoryManager::FactoryManager() {}

void FactoryManager::loadFactories(const std::filesystem::path& file_path) {
    if (file_path.extension() != ".so") {
        throw std::runtime_error("Invalid file format: " + file_path.filename().string());
    }

    void* const handle = dlopen(file_path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!handle) {
        throw std::runtime_error("Failed to open " + file_path.filename().string());
    }

    void (*registerFactories)(FactoryManager*);
    registerFactories = (void (*)(FactoryManager*))dlsym(handle, "registerFactories");

    if (const char* error = dlerror()) {
        throw std::runtime_error("Failed to load " + file_path.filename().string()
                                 + ", error: " + std::string(error));
    }

    try {
        registerFactories(this);
    } catch (const std::exception& exception) {
        throw std::runtime_error("Failed to register factories in "
                                 + file_path.filename().string());
    }
}

std::unique_ptr<SolutionBase> FactoryManager::createSolution(const std::string& name) const {
    if (!m_solution_factories.contains(name)) {
        throw std::runtime_error("Factory " + name + " not registered");
    }
    return m_solution_factories.at(name)->create();
}

std::map<std::string, std::unique_ptr<SolutionBase>> FactoryManager::createAllSolutions()
    const {
    std::map<std::string, std::unique_ptr<SolutionBase>> solutions;
    for (const auto& [name, factory] : m_solution_factories) {
        solutions[name] = factory->create();
    }
    return solutions;
}

}  // namespace aoc