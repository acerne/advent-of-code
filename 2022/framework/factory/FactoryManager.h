#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "Factory.h"
#include "Solution.h"

namespace aoc {

class FactoryManager {
  public:  // Constructors
    FactoryManager();

  public:  // Methods
    void loadFactories(const std::filesystem::path& file_path);

    template <typename T>
    void registerSolution(const std::string& name) {
        if (m_solution_factories.contains(name)) {
            throw std::runtime_error("Duplicate factories with name " + name);
        }
        std::unique_ptr<FactoryBase<Solution>> factory =
            std::make_unique<Factory<Solution, T>>(name);
        m_solution_factories.emplace(name, std::move(factory));
    }

    std::unique_ptr<Solution> createSolution(const std::string& name) const;
    std::map<std::string, std::unique_ptr<Solution>> createAllSolutions() const;

  private:  // Members
    std::map<std::string, std::unique_ptr<FactoryBase<Solution>>> m_solution_factories;
};

}  // namespace aoc