#include "Day.h"

#include "FactoryManager.h"
#include "helpers.h"

#include <set>

namespace aoc {

inline int mod(int a, int b) {
    return (a % b + b) % b;
}

struct Point {
    int x{0};
    int y{0};

    bool operator<(const Point& rhs) const {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }

    friend Point operator+(const Point& lhs, const Point& rhs) {
        return Point{lhs.x + rhs.x, lhs.y + rhs.y};
    }
};

enum class Turn {
    Right,
    Left,
    None,
};

enum class Dir {
    N,
    S,
    W,
    E,
};

struct Instruction {
    int steps;
    Turn turn;
};

struct Tile {
    // A tile of 4x4 grid
    //  Note: cube can be unrolled to a 5x2 (or 2x5) grid, ignoring that.
    const int tile{-1};
    const int size;
    std::set<Point> walls;

    bool isWall(const Point& pos) const {
        return walls.contains(pos);
    }

    bool isOutside(const Point& pos) const {
        return pos.x < 0 || pos.x >= size || pos.y < 0 || pos.y >= size;
    }
};

class Board {
  protected:
    struct Player {
        int tile{-1};
        Point pos;
        Dir orientation;
    };

  public:
    Board(const std::map<int, Tile>& tiles)
          : m_tiles(tiles),
            m_tile_size(tiles.begin()->second.size) {
        // Starting position
        m_player.orientation = Dir::E;
        for (int i = 0; i < 4; i++) {
            if (m_tiles.contains(i)) {
                Point start;
                for (start.x = 0; start.x < m_tile_size; start.x++) {
                    if (!m_tiles.at(i).isWall(start)) {
                        m_player.pos = start;
                        m_player.tile = i;
                        break;
                    }
                }
            }
            if (m_player.tile != -1) {
                break;
            }
        }
    }

  public:
    void move(int steps) {
        static const std::map<Dir, Point> unit_step{
            {Dir::N, {0, -1}},
            {Dir::S, {0, 1} },
            {Dir::W, {-1, 0}},
            {Dir::E, {1, 0} },
        };
        for (int i = 0; i < steps; i++) {
            Point step = m_player.pos + unit_step.at(m_player.orientation);
            if (m_tiles.at(m_player.tile).isWall(step)) {
                break;
            } else if (m_tiles.at(m_player.tile).isOutside(step)) {
                Player candidate = carryOverEdge(m_player);
                if (m_tiles.at(candidate.tile).isWall(candidate.pos)) {
                    break;
                } else {
                    m_player = candidate;
                }
            } else {
                m_player.pos = step;
            }
        }
    }

    void turn(Turn turn) {
        static const std::map<Turn, std::map<Dir, Dir>> turns{
            {
             Turn::Right,
             {
                    {Dir::W, Dir::N},
                    {Dir::N, Dir::E},
                    {Dir::E, Dir::S},
                    {Dir::S, Dir::W},
                }, },
            {
             Turn::Left,
             {
                    {Dir::W, Dir::S},
                    {Dir::S, Dir::E},
                    {Dir::E, Dir::N},
                    {Dir::N, Dir::W},
                }, },
        };
        if (turn != Turn::None) {
            m_player.orientation = turns.at(turn).at(m_player.orientation);
        }
    }

    int score() const {
        static const std::map<Dir, int> facing{
            {Dir::E, 0},
            {Dir::S, 1},
            {Dir::W, 2},
            {Dir::N, 3},
        };
        const int row = m_player.pos.y + (m_player.tile / 4) * m_tile_size;
        const int col = m_player.pos.x + (m_player.tile % 4) * m_tile_size;
        return 1000 * (row + 1) + 4 * (col + 1) + facing.at(m_player.orientation);
    }

  protected:
    virtual Player carryOverEdge(const Player& current) const = 0;

  protected:
    const int m_tile_size;
    const std::map<int, Tile> m_tiles;

  private:
    Player m_player;
};

class FlatBoard : public Board {  // Part 1
  public:
    FlatBoard(const std::map<int, Tile>& tiles)
          : Board(tiles) {}

  protected:
    Player carryOverEdge(const Player& current) const override {
        Player next = current;
        switch (next.orientation) {
            case Dir::N: {
                do {
                    next.tile = mod(next.tile - 4, 16);
                } while (!m_tiles.contains(next.tile));
                next.pos.y = m_tile_size - 1;
            } break;
            case Dir::S: {
                do {
                    next.tile = mod(next.tile + 4, 16);
                } while (!m_tiles.contains(next.tile));
                next.pos.y = 0;
            } break;
            case Dir::W: {
                do {
                    int offset = (next.tile / 4) * 4;
                    next.tile = mod(next.tile - offset - 1, 4) + offset;
                } while (!m_tiles.contains(next.tile));
                next.pos.x = m_tile_size - 1;
            } break;
            case Dir::E: {
                do {
                    int offset = (next.tile / 4) * 4;
                    next.tile = mod(next.tile - offset + 1, 4) + offset;
                } while (!m_tiles.contains(next.tile));
                next.pos.x = 0;
            } break;
            default:
                break;
        }
        return next;
    }
};

class CubeBoard : public Board {  // Part 2
    using Edge = std::pair<int, Dir>;

  public:
    CubeBoard(const std::map<int, Tile>& tiles)
          : Board(tiles) {
        // Hard coded box folding      D        opposite sides:
        //                         +----+        A-B - top/bottom
        //  sample   input        / A  /|        C-D - front/back
        //  |  A     | AF        +----+ | F      E-F - left/right
        //  |DEC     | C       E | C  | +
        //  |  BF    |EB         |    |/
        //           |D          +----+
        //                          B
        //
        // Directions (N, E, S, W) are from perspective of the grid map

        int A, B, C, D, E, F;
        std::vector<std::pair<Edge, Edge>> edges;
        if (tiles.contains(1)) {  // Input
            A = 1;
            B = 9;
            C = 5;
            D = 12;
            E = 8;
            F = 2;
            edges = {
                {{A, Dir::N}, {D, Dir::W}}, // Top edges (A)
                {{A, Dir::E}, {F, Dir::W}},
                {{A, Dir::S}, {C, Dir::N}},
                {{A, Dir::W}, {E, Dir::W}},
                {{B, Dir::N}, {C, Dir::S}}, // Bottom edges (B)
                {{B, Dir::E}, {F, Dir::E}},
                {{B, Dir::S}, {D, Dir::E}},
                {{B, Dir::W}, {E, Dir::E}},
                {{D, Dir::S}, {F, Dir::N}}, // Side edges (D-F-C-E)
                {{F, Dir::S}, {C, Dir::E}},
                {{C, Dir::W}, {E, Dir::N}},
                {{E, Dir::S}, {D, Dir::N}},
            };
        } else {  // Sample
            A = 2;
            B = 10;
            C = 6;
            D = 4;
            E = 5;
            F = 11;
            edges = {
                {{A, Dir::N}, {D, Dir::N}}, // Top edges (A)
                {{A, Dir::E}, {F, Dir::E}},
                {{A, Dir::S}, {C, Dir::N}},
                {{A, Dir::W}, {E, Dir::N}},
                {{B, Dir::N}, {C, Dir::S}}, // Bottom edges (B)
                {{B, Dir::E}, {F, Dir::W}},
                {{B, Dir::S}, {D, Dir::S}},
                {{B, Dir::W}, {E, Dir::S}},
                {{D, Dir::W}, {F, Dir::S}}, // Side edges (D-F-C-E)
                {{F, Dir::N}, {C, Dir::E}},
                {{C, Dir::W}, {E, Dir::E}},
                {{E, Dir::W}, {D, Dir::E}},
            };
        }
        for (const auto& [from, to] : edges) {
            m_cubemap[from] = to;
            m_cubemap[to] = from;
        }
        if (m_cubemap.size() != 24) {
            throw std::runtime_error("Oops");
        }
    }

  protected:
    Player carryOverEdge(const Player& current) const override {
        static const std::map<Dir, Dir> dir_switch{
            {Dir::N, Dir::S},
            {Dir::S, Dir::N},
            {Dir::W, Dir::E},
            {Dir::E, Dir::W},
        };
        const auto [next_tile, connecting_edge] =
            m_cubemap.at({current.tile, current.orientation});
        Player next = current;
        next.tile = next_tile;
        next.orientation = dir_switch.at(connecting_edge);
        switch (current.orientation) {
            case Dir::N: {
                switch (connecting_edge) {
                    case Dir::N: {  // N -> N
                        next.pos.x = m_tile_size - 1 - current.pos.x;
                        next.pos.y = 0;
                    } break;
                    case Dir::S: {  // N -> S
                        next.pos.y = m_tile_size - 1;
                    } break;
                    case Dir::W: {  // N -> W
                        next.pos.x = 0;
                        next.pos.y = current.pos.x;
                    } break;
                    case Dir::E: {  // N -> E
                        next.pos.x = m_tile_size - 1;
                        next.pos.y = m_tile_size - 1 - current.pos.x;
                    } break;
                    default:
                        break;
                }
            } break;
            case Dir::S: {
                switch (connecting_edge) {
                    case Dir::N: {  // S -> N
                        next.pos.y = 0;
                    } break;
                    case Dir::S: {  // S -> S
                        next.pos.x = m_tile_size - 1 - current.pos.x;
                        next.pos.y = m_tile_size - 1;
                    } break;
                    case Dir::W: {  // S -> W
                        next.pos.x = 0;
                        next.pos.y = m_tile_size - 1 - current.pos.x;
                    } break;
                    case Dir::E: {  // S -> E
                        next.pos.x = m_tile_size - 1;
                        next.pos.y = current.pos.x;
                    } break;
                    default:
                        break;
                }
            } break;
            case Dir::W: {
                switch (connecting_edge) {
                    case Dir::N: {  // W -> N
                        next.pos.x = current.pos.y;
                        next.pos.y = 0;
                    } break;
                    case Dir::S: {  // W -> S
                        next.pos.x = m_tile_size - 1 - current.pos.y;
                        next.pos.y = m_tile_size - 1;
                    } break;
                    case Dir::W: {  // W -> W
                        next.pos.x = 0;
                        next.pos.y = m_tile_size - 1 - current.pos.y;
                    } break;
                    case Dir::E: {  // W -> E
                        next.pos.x = m_tile_size - 1;
                    } break;
                    default:
                        break;
                }
            } break;
            case Dir::E: {
                switch (connecting_edge) {
                    case Dir::N: {  // E -> N
                        next.pos.x = m_tile_size - 1 - current.pos.y;
                        next.pos.y = 0;
                    } break;
                    case Dir::S: {  // E -> S
                        next.pos.x = current.pos.y;
                        next.pos.y = m_tile_size - 1;
                    } break;
                    case Dir::W: {  // E -> W
                        next.pos.x = 0;
                    } break;
                    case Dir::E: {  // E -> E
                        next.pos.x = m_tile_size - 1;
                        next.pos.y = m_tile_size - 1 - current.pos.y;
                    } break;
                    default:
                        break;
                }
            } break;
            default:
                break;
        }
        return next;
    }

  private:
    std::map<Edge, Edge> m_cubemap;
};

Input Day::read(std::ifstream& line_stream) const {
    Input inputs;
    std::string line;
    // Parse map
    std::vector<std::string> lines;
    size_t x_max{0};
    while (std::getline(line_stream, line)) {
        if (line.empty()) {
            break;
        }
        lines.push_back(line);
        x_max = std::max(x_max, line.size());
    }
    const int tile_size = std::max(x_max, lines.size()) / 4;
    for (int y = 0; y < lines.size(); y++) {
        for (int x = 0; x < lines.at(y).size(); x++) {
            const int tile_idx = y / tile_size * 4 + x / tile_size;
            const char c = lines.at(y).at(x);
            if (!inputs.first.contains(tile_idx) && (c == '#' || c == '.')) {
                inputs.first.insert({
                    tile_idx, Tile{.tile = tile_idx, .size = tile_size}
                });
            }
            if (c == '#') {
                Tile& tile = inputs.first.at(y / tile_size * 4 + x / tile_size);
                tile.walls.insert({x % tile_size, y % tile_size});
            }
        }
    }

    // Parse instructions
    std::getline(line_stream, line);
    size_t pos{0};
    while (pos < line.size() - 1) {
        size_t r = line.find_first_of('R', pos);
        size_t l = line.find_first_of('L', pos);
        if (r < l) {
            inputs.second.push_back({std::stoi(line.substr(pos, r - pos)), Turn::Right});
            pos = r + 1;
        } else {
            inputs.second.push_back({std::stoi(line.substr(pos, l - pos)), Turn::Left});
            pos = l + 1;
        }
    }
    // Last value has no turn instruction
    inputs.second.push_back({std::stoi(line.substr(pos, line.size() - pos)), Turn::None});
    return inputs;
}

Result Day::partOne(const Input& input) const {
    FlatBoard board(input.first);
    for (const auto& instruction : input.second) {
        board.move(instruction.steps);
        board.turn(instruction.turn);
    }
    return board.score();
}

Result Day::partTwo(const Input& input) const {
    CubeBoard board(input.first);
    for (const auto& instruction : input.second) {
        board.move(instruction.steps);
        board.turn(instruction.turn);
    }
    return board.score();
}

extern "C" void registerFactories(FactoryManager* manager) {
    manager->registerSolution<Day>(YEAR + "-" + DAY);
}

}  // namespace aoc