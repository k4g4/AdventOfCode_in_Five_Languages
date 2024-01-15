#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

// #define INPUT "example"
#define INPUT "input"

typedef array<int, 3> Pos;

template <>
struct std::hash<Pos> {
    size_t operator()(const Pos& pos) const noexcept {
        size_t seed = 0;
        for (int i = 0; i < 3; i++) {
            seed ^=
                hash<int>{}(pos[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;
    regex num{"-?\\d+"};
    smatch match;

    ulong sides = 0;
    unordered_set<Pos> positions;

    while (getline(input, line)) {
        sides += 6;

        Pos pos;
        regex_search(line, match, num);
        pos[0] = stoul(match[0].str());
        regex_search(match.suffix().first, line.cend(), match, num);
        pos[1] = stoul(match[0].str());
        regex_search(match.suffix().first, line.cend(), match, num);
        pos[2] = stoul(match[0].str());

        for (int i = 0; i < 3; i++) {
            for (int offset : array<int, 2>{-1, 1}) {
                Pos neighbor = pos;
                neighbor[i] += offset;
                if (positions.find(neighbor) != positions.end()) {
                    sides -= 2;
                }
            }
        }

        positions.insert(pos);
    }

    cout << "sides: " << sides << endl;

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}