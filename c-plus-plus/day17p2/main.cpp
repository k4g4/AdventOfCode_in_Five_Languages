#include <array>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace std::chrono;

// #define INPUT "example"
#define INPUT "input"

#define WIDTH 7
#define MEMO_SIZE 25
#define TOTAL_ROCKS 1'000'000'000'000

struct Coord {
    int x;
    int y;

    Coord() {}
    Coord(int x, int y) : x{x}, y{y} {}
};

class Rock {
    array<Coord, 5> coords;
    size_t size;

   public:
    typedef array<Coord, 5>::iterator iterator;

    Rock() {}
    Rock(Coord first, Coord second, Coord third, Coord fourth)
        : coords{first, second, third, fourth}, size{4} {}
    Rock(Coord first, Coord second, Coord third, Coord fourth, Coord fifth)
        : coords{first, second, third, fourth, fifth}, size{5} {}

    iterator begin() { return coords.begin(); }
    iterator end() { return coords.end() - (5 - size); }
};

Rock nextRock(ulong rocksFallen) {
    switch (rocksFallen % 5) {
        case 0:
            return {{2, 0}, {3, 0}, {4, 0}, {5, 0}};
        case 1:
            return {{3, 0}, {2, 1}, {3, 1}, {4, 1}, {3, 2}};
        case 2:
            return {{2, 0}, {3, 0}, {4, 0}, {4, 1}, {4, 2}};
        case 3:
            return {{2, 0}, {2, 1}, {2, 2}, {2, 3}};
        default:
            return {{2, 0}, {3, 0}, {2, 1}, {3, 1}};
    }
}

bool validRock(const vector<bitset<WIDTH>> &rows, Rock rock) {
    for (auto coord : rock) {
        if (coord.y < 0 || coord.x < 0 || coord.x >= WIDTH) {
            return false;
        }
        if (coord.y < rows.size()) {
            if (rows[coord.y][coord.x]) {
                return false;
            }
        }
    }
    return true;
}

typedef array<bitset<WIDTH>, MEMO_SIZE> memokey;

memokey getKey(const vector<bitset<WIDTH>> &rows) {
    memokey key;
    copy(rows.rbegin(), rows.rbegin() + min(size_t{MEMO_SIZE}, rows.size()),
         key.begin());
    return key;
}

template <>
struct std::hash<memokey> {
    size_t operator()(const memokey &key) const noexcept {
        size_t seed = 0;
        for (auto i : key) {
            seed ^= hash<bitset<WIDTH>>{}(i) + 0x9e3779b9 + (seed << 6) +
                    (seed >> 2);
        }
        return seed;
    }
};

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;
    getline(input, line);

    vector<bitset<WIDTH>> rows;
    Rock rock;
    ulong rocksFallen = 0;
    bool dropRock = true;
    long height = 0;
    long trueHeight = 0;
    unordered_map<memokey, tuple<ulong, long, ulong>> memo;
    bool usingMemo = true;

    for (ulong tick = 0;; tick++) {
        if (dropRock) {
            rock = nextRock(rocksFallen++);
            for (auto &coord : rock) {
                coord.y += height + 3;
            }
            dropRock = false;
        }

        Rock pushed = rock;
        for (auto &coord : pushed) {
            coord.x += line[tick % line.size()] == '>' ? 1 : -1;
        }
        if (validRock(rows, pushed)) {
            rock = pushed;
        }

        Rock dropped = rock;
        for (auto &coord : dropped) {
            coord.y--;
        }
        if (validRock(rows, dropped)) {
            rock = dropped;
        } else {
            for (auto coord : rock) {
                auto prevHeight = height;
                height = max(height, long{coord.y + 1});
                trueHeight += height - prevHeight;
                while (coord.y >= rows.size()) {
                    rows.emplace_back();
                }
                rows[coord.y][coord.x] = true;
            }
            dropRock = true;
            if (rocksFallen == TOTAL_ROCKS) {
                break;
            }

            if (usingMemo) {
                auto key = getKey(rows);
                auto memory = memo.cend();
                if ((memory = memo.find(key)) != memo.cend()) {
                    ulong rocksFallenMem = get<0>(memory->second);
                    long heightMem = get<1>(memory->second);
                    ulong tickMem = get<2>(memory->second);
                    ulong cycleLen = rocksFallen - rocksFallenMem;
                    if (cycleLen < 5) {
                        continue;
                    }
                    ulong cycles = (TOTAL_ROCKS - rocksFallenMem) / cycleLen;
                    rocksFallen = cycles * cycleLen + rocksFallenMem;
                    tick = cycles * (tick - tickMem) + tickMem;
                    trueHeight = cycles * (height - heightMem) + heightMem;
                    usingMemo = false;
                } else {
                    memo[key] = make_tuple(rocksFallen, height, tick);
                }
            }
        }
    }

    cout << "height: " << trueHeight << endl;

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}