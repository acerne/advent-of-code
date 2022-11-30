#pragma once

#include <memory>
#include <string>

namespace aoc {

template <typename Base>
class FactoryBase {
  public:  // Constructors
    virtual ~FactoryBase() = default;

  public:  // Methods
    virtual std::unique_ptr<Base> create() = 0;
};

template <typename Base, typename T>
class Factory : public FactoryBase<Base> {
  public:  // Constructors
    explicit Factory(const std::string& id)
          : m_id(id) {}

  public:  // Methods
    std::unique_ptr<Base> create() override {
        std::unique_ptr<Base> obj = std::make_unique<T>();
        return obj;
    }

  private:  // Members
    std::string m_id;
};

}  // namespace aoc