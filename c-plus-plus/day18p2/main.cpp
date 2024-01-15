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

ulong outerSides(const unordered_set<Pos>& positions, Pos minPos, Pos maxPos) {
    ulong sides = 0;
    unordered_set<Pos> checked{minPos};
    vector<Pos> queue{minPos};

    while (!queue.empty()) {
        Pos pos = *queue.rbegin();
        queue.pop_back();

        for (int i = 0; i < 3; i++) {
            for (int offset : array<int, 2>{-1, 1}) {
                Pos neighbor = pos;
                neighbor[i] += offset;
                bool inBounds = true;
                for (int j = 0; j < 3; j++) {
                    if (neighbor[j] > maxPos[j] || neighbor[j] < minPos[j]) {
                        inBounds = false;
                        break;
                    }
                }
                if (inBounds && checked.find(neighbor) == checked.end()) {
                    if (positions.find(neighbor) == positions.end()) {
                        checked.insert(neighbor);
                        queue.push_back(neighbor);
                    } else {
                        sides++;
                    }
                }
            }
        }
    }

    return sides;
}

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;
    regex num{"-?\\d+"};
    smatch match;

    unordered_set<Pos> positions;
    Pos minPos{0, 0, 0};
    Pos maxPos{0, 0, 0};

    while (getline(input, line)) {
        Pos pos;

        regex_search(line, match, num);
        pos[0] = stoul(match[0].str());
        minPos[0] = min(minPos[0], pos[0]);
        maxPos[0] = max(maxPos[0], pos[0]);

        regex_search(match.suffix().first, line.cend(), match, num);
        pos[1] = stoul(match[0].str());
        minPos[1] = min(minPos[1], pos[1]);
        maxPos[1] = max(maxPos[1], pos[1]);

        regex_search(match.suffix().first, line.cend(), match, num);
        pos[2] = stoul(match[0].str());
        minPos[2] = min(minPos[2], pos[2]);
        maxPos[2] = max(maxPos[2], pos[2]);

        positions.insert(pos);
    }

    for (int i = 0; i < 3; i++) {
        minPos[i]--;
        maxPos[i]++;
    }

    cout << "sides: " << outerSides(positions, minPos, maxPos) << endl;

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}