#include <array>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

// #define INPUT "example"
#define INPUT "input"

#define WIDTH 7
#define TOTAL_ROCKS 2022

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

Rock nextRock(uint rocksFallen) {
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

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;
    getline(input, line);

    vector<bitset<WIDTH>> rows;
    Rock rock;
    uint rocksFallen = 0;
    bool dropRock = true;
    int height = 0;

    for (uint tick = 0;; tick = (tick + 1) % line.size()) {
        if (dropRock) {
            rock = nextRock(rocksFallen++);
            for (auto &coord : rock) {
                coord.y += height + 3;
            }
            dropRock = false;
        }

        Rock pushed = rock;
        for (auto &coord : pushed) {
            coord.x += line[tick] == '>' ? 1 : -1;
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
                height = max(height, coord.y + 1);
                while (coord.y >= rows.size()) {
                    rows.emplace_back();
                }
                rows[coord.y][coord.x] = true;
            }
            dropRock = true;
            if (rocksFallen == TOTAL_ROCKS) {
                break;
            }
        }
    }

    cout << "height: " << height << endl;

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}