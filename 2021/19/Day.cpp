#include "Day.h"

#include "FactoryManager.h"

#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <set>

namespace aoc {

struct Vector3D {
    int32_t x;
    int32_t y;
    int32_t z;

    Vector3D()
          : x(0),
            y(0),
            z(0) {}

    Vector3D(int32_t x, int32_t y, int32_t z)
          : x(x),
            y(y),
            z(z) {}

    Vector3D transform(int rot) const {
        switch (rot) {
            case 1: {
                return Vector3D(x, y, z);
            } break;
            case 2: {
                return Vector3D(x, -z, y);
            } break;
            case 3: {
                return Vector3D(x, -y, -z);
            } break;
            case 4: {
                return Vector3D(x, z, -y);
            } break;
            case 5: {
                return Vector3D(-y, x, z);
            } break;
            case 6: {
                return Vector3D(z, x, y);
            } break;
            case 7: {
                return Vector3D(y, x, -z);
            } break;
            case 8: {
                return Vector3D(-z, x, -y);
            } break;
            case 9: {
                return Vector3D(-x, -y, z);
            } break;
            case 10: {
                return Vector3D(-x, -z, -y);
            } break;
            case 11: {
                return Vector3D(-x, y, -z);
            } break;
            case 12: {
                return Vector3D(-x, z, y);
            } break;
            case 13: {
                return Vector3D(y, -x, z);
            } break;
            case 14: {
                return Vector3D(z, -x, -y);
            } break;
            case 15: {
                return Vector3D(-y, -x, -z);
            } break;
            case 16: {
                return Vector3D(-z, -x, y);
            } break;
            case 17: {
                return Vector3D(-z, y, x);
            } break;
            case 18: {
                return Vector3D(y, z, x);
            } break;
            case 19: {
                return Vector3D(z, -y, x);
            } break;
            case 20: {
                return Vector3D(-y, -z, x);
            } break;
            case 21: {
                return Vector3D(-z, -y, -x);
            } break;
            case 22: {
                return Vector3D(-y, z, -x);
            } break;
            case 23: {
                return Vector3D(z, y, -x);
            } break;
            case 24: {
                return Vector3D(y, -z, -x);
            } break;
            default:
                throw std::runtime_error("Invalid state");
        }
    }

    int32_t manhattan() const {
        return abs(x) + abs(y) + abs(z);
    }

    int32_t manhattan(const Vector3D& v) const {
        return abs(x - v.x) + abs(y - v.y) + abs(z - v.z);
    }

    bool operator==(const Vector3D& rhs) const {
        return std::tie(x, y, z) == std::tie(rhs.x, rhs.y, rhs.z);
    }

    bool operator<(const Vector3D& rhs) const {
        return std::tie(x, y, z) < std::tie(rhs.x, rhs.y, rhs.z);
    }

    friend Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs) {
        return Vector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }

    friend Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs) {
        return Vector3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& p) {
        return os << "( " << p.x << ", " << p.y << ", " << p.z << " )";
    }
};

struct LocalCluster {
    std::map<Vector3D, std::vector<Vector3D>> beacons;
    int id;

    LocalCluster(const std::vector<Vector3D>& local_beacons) {
        static int counter = 0;
        id = counter++;
        for (const auto& a : local_beacons) {
            std::vector<Vector3D> ab_distances;
            ab_distances.reserve(local_beacons.size());
            for (const auto& b : local_beacons) {
                ab_distances.push_back(b - a);
            }
            beacons[a] = std::move(ab_distances);
        }
    }
};

class GlobalCluster {
  public:
    GlobalCluster(const LocalCluster& local) {
        add(local, Vector3D(0, 0, 0), 1);
    }

    std::pair<int, std::optional<Vector3D>> score(const LocalCluster& local, int rot) {
        // Calculate the matching score of the local cluster with the global cluster
        int match = 0;
        std::optional<Vector3D> location_candidate;
        for (const auto& [local_beacon, local_distances] : local.beacons) {
            for (const auto& local_distance : local_distances) {
                bool found = false;
                if (local_distance.manhattan() == 0) {
                    continue;
                }
                for (const auto& [global_beacon, global_distances] : m_beacons) {
                    for (const auto& global_distance : global_distances) {
                        if (global_distance.manhattan() == 0) {
                            continue;
                        }
                        auto rotated_distance = local_distance.transform(rot);
                        if (global_distance == rotated_distance) {
                            found = true;
                            location_candidate = global_beacon - local_beacon.transform(rot);
                            break;
                        }
                    }
                    if (found) {
                        match++;
                        break;
                    }
                }
            }
        }

        return {match, location_candidate};
    }

    void add(const LocalCluster& local, const Vector3D& scanner_global_position, int rot) {
        m_scanners.push_back(scanner_global_position);
        // Convert beacons and insert them to map
        for (const auto& [local_beacon, _] : local.beacons) {
            auto rotated_beacon = local_beacon.transform(rot);
            auto global_beacon = scanner_global_position + rotated_beacon;
            m_beacons[global_beacon] = std::vector<Vector3D>();
        }
        // Recalculate the distances
        for (auto& [a, ab_distances] : m_beacons) {
            ab_distances.clear();
            ab_distances.reserve(m_beacons.size());
            for (const auto& [b, _] : m_beacons) {
                ab_distances.push_back(b - a);
            }
        }
    }

    size_t nBeacons() const {
        return m_beacons.size();
    }

    size_t maxScannerDistance() const {
        size_t max = 0;
        for (const auto& a : m_scanners) {
            for (const auto& b : m_scanners) {
                auto d = a.manhattan(b);
                if (d > max) {
                    max = d;
                }
            }
        }
        return max;
    }

  private:
    std::vector<Vector3D> m_scanners;
    std::map<Vector3D, std::vector<Vector3D>> m_beacons;
};

std::vector<std::string> split(const std::string& line, char delimiter) {
    size_t start = 0;
    size_t end;
    std::vector<std::string> tokens;

    while ((end = line.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(line.substr(start));

    return tokens;
}

GlobalCluster placeScanners(std::vector<LocalCluster>& scanner_clusters) {
    GlobalCluster global(scanner_clusters.front());
    scanner_clusters.erase(scanner_clusters.begin());

    while (!scanner_clusters.empty()) {
        int best_match = -1;
        int best_rot;
        std::vector<LocalCluster>::iterator best_it;
        Vector3D best_estimation;
        for (auto it = scanner_clusters.begin(); it != scanner_clusters.end(); ++it) {
            std::cout << it->id << std::endl;
            for (int rot = 1; rot < 25; rot++) {
                auto [match, scanner] = global.score(*it, rot);
                if (match > best_match && scanner) {
                    best_match = match;
                    best_rot = rot;
                    best_it = it;
                    best_estimation = scanner.value();
                }
            }
        }
        std::cout << "Best: " << best_match << std::endl;
        global.add(*best_it, best_estimation, best_rot);
        scanner_clusters.erase(best_it);
    }
}

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    std::vector<Vector3D> beacons;
    while (std::getline(line_stream, line)) {
        if (line.empty()) {
            inputs.push_back(beacons);
        } else if (line.find("---") != std::string::npos) {
            beacons.clear();
        } else {
            auto tokens = split(line, ',');
            beacons.emplace_back(std::stoi(tokens.at(0)), std::stoi(tokens.at(1)),
                                 std::stoi(tokens.at(2)));
        }
    }
    if (!beacons.empty()) {
        inputs.push_back(beacons);
    }
    return inputs;
}

Result Day::partOne(const Input& input) const {
    std::vector<LocalCluster> scanner_clusters;
    for (const auto& item : input) {
        scanner_clusters.emplace_back(item);
    }

    GlobalCluster global = placeScanners(scanner_clusters);

    return global.maxScannerDistance();
}

Result Day::partTwo(const Input& input) const {
    std::vector<LocalCluster> scanner_clusters;
    for (const auto& item : input) {
        scanner_clusters.emplace_back(item);
    }

    GlobalCluster global = placeScanners(scanner_clusters);

    return global.maxScannerDistance();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc