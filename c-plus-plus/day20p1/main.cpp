#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace std::chrono;

// #define INPUT "example"
// #define NODES 7
#define INPUT "input"
#define NODES 5000

struct Node {
    uint prev;
    uint next;
    int val;
};

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;
    array<Node, NODES> nodes;

    for (uint i = 0; i < NODES; i++) {
        getline(input, line);
        nodes[i] = Node{
            .prev = (i + NODES - 1) % NODES,
            .next = (i + 1) % NODES,
            .val = stoi(line),
        };
    }

    for (uint i = 0; i < NODES; i++) {
        if (nodes[i].val % NODES) {
            nodes[nodes[i].prev].next = nodes[i].next;
            nodes[nodes[i].next].prev = nodes[i].prev;

            uint at = i;
            if (nodes[i].val > 0) {
                for (uint j = 0; j < nodes[i].val % (NODES - 1); j++) {
                    at = nodes[at].next;
                }
                nodes[i].next = nodes[at].next;
                nodes[nodes[at].next].prev = i;
                nodes[i].prev = at;
                nodes[at].next = i;
            } else {
                for (uint j = 0;
                     j < (-nodes[i].val + (NODES - 1)) % (NODES - 1); j++) {
                    at = nodes[at].prev;
                }
                nodes[i].prev = nodes[at].prev;
                nodes[nodes[at].prev].next = i;
                nodes[i].next = at;
                nodes[at].prev = i;
            }
        }
    }

    array<int, NODES> mixed;
    for (uint i = 0, at = 0; i < NODES; i++) {
        mixed[i] = nodes[at].val;
        at = nodes[at].next;
    }
    uint pos = find(mixed.begin(), mixed.end(), 0) - mixed.begin();
    int sum = 0;
    for (uint offset : array<uint, 3>{1000, 2000, 3000}) {
        sum += mixed[(pos + offset) % NODES];
    }

    cout << "sum of grove coordinates: " << sum << endl;

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}