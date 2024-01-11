#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define INPUT "example"

using namespace std;
using namespace std::chrono;

struct Name : array<char, 2> {
    Name(const smatch& match) {
        (*this)[0] = *match[0].first;
        (*this)[1] = *(match[0].first + 1);
    }
};

template <>
struct std::hash<Name> {
    size_t operator()(const Name& name) const noexcept {
        return hash<char>{}(name[0]) ^ (hash<char>{}(name[1]));
    }
};

ostream& operator<<(ostream& os, const Name& name) {
    return os << name[0] << name[1];
}

struct Valve {
    int flowRate;
    vector<Name> tunnels;
};

ostream& operator<<(ostream& os, const Valve& valve) {
    os << "flow rate: " << valve.flowRate << " tunnels: '";
    for (const auto& name : valve.tunnels) {
        os << name << ", ";
    }
    return os << "'";
}

uint recurse(const unordered_map<Name, Valve>& valves, Name currentValve,
             unordered_set<Name> released, uint currentFlow, size_t currentHash,
             unordered_map<size_t, uint> memo) {
    //
}

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;

    const regex namePattern{"[A-Z]{2}"};
    const regex flowRatePattern{"\\d+"};
    smatch match;

    unordered_map<Name, Valve> valves;

    while (getline(input, line)) {
        Valve valve;

        regex_search(line, match, namePattern);
        Name name{match};

        for (auto remainder = match.suffix().first;
             regex_search(remainder, line.cend(), match, namePattern);
             remainder = match.suffix().first) {
            valve.tunnels.push_back({match});
        }

        regex_search(line, match, flowRatePattern);
        valve.flowRate = stoi(match.str());

        valves[name] = move(valve);
    }

    for (const auto& nameAndValve : valves) {
        cout << "name: " << nameAndValve.first << "\nvalve:\n"
             << nameAndValve.second << '\n';
    }

    unordered_set<Name> released;
    unordered_map<size_t, uint> memo;
    uint flow = 0;

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}