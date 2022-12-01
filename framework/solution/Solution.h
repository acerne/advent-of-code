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
    explicit Solution(std::string year, std::string day, const Result& partOne,
                      const Result& partTwo)
          : m_year(std::move(year)),
            m_day(std::move(day)),
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
    static bool equals(const T& result, const T& expected);

    template <typename T>
    static bool equals(const std::vector<T>& result, const std::vector<T>& expected);

  private:  // Members
    const std::string m_year;
    const std::string m_day;
    const std::map<Part, Result> m_expected;
    std::map<Part, Result> m_result;
    Input m_sample;
    Input m_input;
};

template <typename Input, typename Result>
void Solution<Input, Result>::load() {
    std::string sample_path = "../../" + m_year + "/" + m_day + "/data/sample.txt";
    std::ifstream sample_stream(sample_path);
    if (!sample_stream.good()) {
        throw std::runtime_error("File " + sample_path + " does not exist");
    }
    m_sample = read(sample_stream);

    std::string input_path = "../../" + m_year + "/" + m_day + "/data/input.txt";
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
    oss << "}";
    return oss.str();
}

template <typename Input, typename Result>
template <typename T>
std::string Solution<Input, Result>::resultToString(const T& result) {
    std::ostringstream oss;
    oss << result;
    return oss.str();
}

template <typename Input, typename Result>
template <typename T>
bool Solution<Input, Result>::equals(const T& result, const T& expected) {
    return result == expected;
}

template <typename Input, typename Result>
template <typename T>
bool Solution<Input, Result>::equals(const std::vector<T>& result,
                                     const std::vector<T>& expected) {
    if (result.size() != expected.size()) {
        return false;
    } else {
        for (size_t i = 0; i < result.size(); i++) {
            if (expected.at(i) == std::numeric_limits<T>::max()) {
                // Solution 2021-16 has a special case to ignore some values. These are
                // identified as max value in the vector of expected values.
                continue;
            }
            if (result.at(i) != expected.at(i)) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace aoc