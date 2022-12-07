#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <iostream>

namespace aoc {

struct Command {
  public:
    Command() = default;
    virtual ~Command() = default;
};

struct CD : public Command {
    std::string arg;

    CD(const std::string& arg)
          : arg(arg) {}
};

struct LS : public Command {
    std::vector<std::string> dirs;
    std::vector<std::pair<std::string, size_t>> files;
};

struct Path {
    bool is_dir;
    std::string name;
    size_t size{0};
};

class FileSystem {
    static constexpr size_t DISK_SPACE{70000000};
    static constexpr size_t REQUIRED_SPACE{30000000};

  public:
    FileSystem() = default;

    void runCommand(const std::shared_ptr<Command>& command) {
        if (auto cd = std::dynamic_pointer_cast<CD>(command)) {
            if (cd->arg == "/") {
                m_current_dir = "/";
                m_fs[m_current_dir].is_dir = true;
                m_fs[m_current_dir].name = m_current_dir;
            } else if (cd->arg == "..") {
                m_current_dir.erase(m_current_dir.end() - 1);
                auto pos = m_current_dir.find_last_of('/');
                m_current_dir = m_current_dir.substr(0, pos + 1);
            } else {
                m_current_dir += cd->arg + "/";
                m_fs[m_current_dir].is_dir = true;
                m_fs[m_current_dir].name = cd->arg;
            }
        } else if (auto ls = std::dynamic_pointer_cast<LS>(command)) {
            for (const auto& name : ls->dirs) {
                auto dir = m_current_dir + name + '/';
                m_fs[dir].is_dir = true;
                m_fs[dir].name = name;
            }
            for (const auto& [name, size] : ls->files) {
                auto dir = m_current_dir + name;
                m_fs[dir].is_dir = false;
                m_fs[dir].name = name;
                m_fs[dir].size = size;
            }
        }
    }

    size_t getSize(const std::string& path) {
        auto& item = m_fs.at(path);
        if (item.is_dir && item.size == 0) {
            // Index directory contents
            for (const auto& [sub_path, sub_item] : m_fs) {
                if (sub_path != path && sub_path.find(path) == 0) {
                    size_t size = getSize(sub_path);
                    if (!sub_item.is_dir) {
                        item.size += size;
                    }
                }
            }
        }
        return item.size;
    }

    void buildIndex() {
        getSize("/");
    }

    size_t smallDirs() {
        // Solves part 1
        buildIndex();
        size_t sum = 0;
        for (const auto& [path, item] : m_fs) {
            if (item.is_dir && path != "/" && item.size < 100000) {
                sum += item.size;
            }
        }
        return sum;
    }

    size_t freeUp() {
        // Solves part 2
        const size_t used_space = getSize("/");
        const size_t free_space = DISK_SPACE - used_space;
        const size_t must_free = REQUIRED_SPACE - free_space;

        Path best_match;
        size_t space_left_best = std::numeric_limits<size_t>::max();
        for (const auto& [path, item] : m_fs) {
            if (item.is_dir && item.size > must_free) {
                size_t space_left = item.size - must_free;
                if (space_left < space_left_best) {
                    space_left_best = space_left;
                    best_match = item;
                }
            }
        }

        return best_match.size;
    }

    void print() {
        buildIndex();

        size_t largest_path = 0;
        size_t largest_name = 0;
        for (const auto& [path, item] : m_fs) {
            if (path.size() > largest_path) {
                largest_path = path.size();
            }
            if (item.name.size() > largest_name) {
                largest_name = item.name.size();
            }
        }
        for (const auto& [path, item] : m_fs) {
            std::cout << path << ":" << std::string(largest_path + 2 - path.size(), ' ');
            std::cout << (item.is_dir ? "[d] " : "[f] ");
            std::cout << item.name << std::string(largest_name + 2 - item.name.size(), ' ');
            std::cout << item.size << std::endl;
        }
    }

  private:
    std::string m_current_dir;
    std::map<std::string, Path> m_fs;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    while (std::getline(line_stream, line)) {
        auto tokens = helpers::tokenize(line, ' ');
        if (tokens.front() == "$") {
            if (tokens.at(1) == "ls") {
                inputs.push_back(std::make_unique<LS>());
            } else if (tokens.at(1) == "cd") {
                inputs.push_back(std::make_unique<CD>(tokens.at(2)));
            } else {
                throw std::runtime_error("Invalid state");
            }
        } else {  // Must be ls output, which must be last command
            auto ls = std::dynamic_pointer_cast<LS>(inputs.back());
            if (tokens.at(0) == "dir") {
                ls->dirs.push_back(tokens.at(1));
            } else {
                ls->files.push_back({tokens.at(1), std::stoul(tokens.at(0))});
            }
        }
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    FileSystem fs;
    for (const auto& command : input) {
        fs.runCommand(command);
    }
    // fs.print();
    return fs.smallDirs();
}

Result Day::partTwo(const Input& input) const {
    FileSystem fs;
    for (const auto& command : input) {
        fs.runCommand(command);
    }
    // fs.print();
    return fs.freeUp();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc