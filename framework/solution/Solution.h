#pragma once

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace aoc {

enum class Part {
    One,
    Two,
};

class SolutionBase {
  public:  // Constructors
    SolutionBase() = default;
    virtual ~SolutionBase() = default;

  public:  // Pure virtual methods
    virtual void load() = 0;
    virtual bool test(Part part) = 0;
    virtual void solve(Part part) = 0;
    virtual std::string result(Part part) const = 0;
    virtual std::string expected(Part part) const = 0;
};

template <typename Input, typename Result>
class Solution : public SolutionBase {
  public:  // Constructors
    explicit Solution(std::string path, const Result& partOne, const Result& partTwo)
          : m_path(std::move(path)),
            m_expected{
                {Part::One, partOne},
                {Part::Two, partTwo}
    } {}

    virtual ~Solution() = default;

  public:  // Pure virtual methods
    virtual Input read(std::ifstream& line_stream) const = 0;
    virtual Result partOne(const Input& input) const = 0;
    virtual Result partTwo(const Input& input) const = 0;

  public:  // SolutionBase methods
    void load() final override;
    bool test(Part part) final override;
    void solve(Part part) final override;
    std::string result(Part part) const final override;
    std::string expected(Part part) const final override;

  private:  // Methods
    template <typename T>
    static std::string resultToString(const std::vector<T>& result);

    template <typename T>
    static std::string resultToString(const T& result);

    template <typename T>
    static bool equals(const T& a, const T& b);

    template <typename T>
    static bool equals(const std::vector<T>& a, const std::vector<T>& b);

  private:  // Members
    const std::string m_path;
    const std::map<Part, Result> m_expected;
    std::map<Part, Result> m_result;
    Input m_sample;
    Input m_input;
};

template <typename Input, typename Result>
void Solution<Input, Result>::load() {
    std::string sample_path = "../../" + m_path + "data/sample.txt";
    std::ifstream sample_stream(sample_path);
    if (!sample_stream.good()) {
        throw std::runtime_error("File " + sample_path + " does not exist");
    }
    m_sample = read(sample_stream);

    std::string input_path = "../../" + m_path + "data/input.txt";
    std::ifstream input_stream(input_path);
    if (!input_stream.good()) {
        throw std::runtime_error("File " + input_path + " does not exist");
    }
    m_input = read(input_stream);
}

template <typename Input, typename Result>
bool Solution<Input, Result>::test(Part part) {
    switch (part) {
        case Part::One: {
            m_result[part] = partOne(m_sample);
        } break;
        case Part::Two: {
            m_result[part] = partTwo(m_sample);
        } break;
        default:
            throw std::runtime_error("Invalid state");
    }
    return equals(m_result.at(part), m_expected.at(part));
}

template <typename Input, typename Result>
void Solution<Input, Result>::solve(Part part) {
    switch (part) {
        case Part::One: {
            m_result[part] = partOne(m_input);
        } break;
        case Part::Two: {
            m_result[part] = partTwo(m_input);
        } break;
        default:
            throw std::runtime_error("Invalid state");
    }
}

template <typename Input, typename Result>
std::string Solution<Input, Result>::result(Part part) const {
    return resultToString(m_result.at(part));
}

template <typename Input, typename Result>
std::string Solution<Input, Result>::expected(Part part) const {
    return resultToString(m_expected.at(part));
}

template <typename Input, typename Result>
template <typename T>
std::string Solution<Input, Result>::resultToString(const std::vector<T>& result) {
    std::ostringstream oss, fail_report;
    oss << "{ ";
    for (const auto& value : result) {
        oss << value << " ";
    }
    oss << "}" << std::endl;
    return oss.str();
}

template <typename Input, typename Result>
template <typename T>
std::string Solution<Input, Result>::resultToString(const T& result) {
    return std::to_string(result);
}

template <typename Input, typename Result>
template <typename T>
bool Solution<Input, Result>::equals(const T& a, const T& b) {
    return a == b;
}

template <typename Input, typename Result>
template <typename T>
bool Solution<Input, Result>::equals(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        return false;
    } else {
        for (size_t i = 0; i < a.size(); i++) {
            if (a.at(i) != b.at(i)) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace aoc