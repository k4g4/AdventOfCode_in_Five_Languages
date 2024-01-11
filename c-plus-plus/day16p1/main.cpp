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

typedef uint flow_t;

struct Name : array<char, 2> {
    Name(char zero, char one) : array{zero, one} {}
    Name(const smatch &match) {
        (*this)[0] = *match[0].first;
        (*this)[1] = *(match[0].first + 1);
    }
};

template <>
struct std::hash<Name> {
    size_t operator()(const Name &name) const noexcept {
        return hash<char>{}(name[0]) ^ (hash<char>{}(name[1]) << 1);
    }
};

ostream &operator<<(ostream &os, const Name &name) {
    return os << name[0] << name[1];
}

struct Valve {
    flow_t flowRate;
    vector<Name> tunnels;
};

ostream &operator<<(ostream &os, const Valve &valve) {
    os << "flow rate: " << valve.flowRate << " tunnels: '";
    for (const auto &name : valve.tunnels) {
        os << name << ", ";
    }
    return os << "'";
}

flow_t recurse(const unordered_map<Name, Valve> &valves, Name currentValveName,
               unordered_set<Name> released, flow_t currentFlowRate,
               flow_t currentFlow, uint minutesRemaining, size_t currentHash,
               unordered_map<size_t, flow_t> &memo) {
    ofstream out{"out", std::ios::app};
    out << "at: " << currentValveName << "\nwith flow: " << currentFlow
        << "\nflow rate: " << currentFlowRate << "\nmins: " << minutesRemaining
        << "\nreleased: ";
    for (auto name : released) {
        out << name << ", ";
    }
    out << "\n\n";
    decltype(memo.cbegin()) memory;
    if ((memory = memo.find(currentHash)) != memo.cend()) {
        return memory->second;
    }

    auto newFlow = currentFlow + currentFlowRate;
    if (!--minutesRemaining) {
        return newFlow;
    }

    size_t newHash;
    flow_t maxFlow = 0;
    auto &valve = valves.at(currentValveName);

    for (auto newValveName : valve.tunnels) {
        newHash = currentHash ^ (hash<Name>{}(newValveName) << 1);
        maxFlow = max(maxFlow,
                      recurse(valves, newValveName, released, currentFlowRate,
                              newFlow, minutesRemaining, newHash, memo));
    }

    // if this valve hasn't been released yet
    if (released.find(currentValveName) == released.end()) {
        auto newFlowRate = currentFlowRate + valve.flowRate;
        released.insert(currentValveName);
        // rehashing current name represents turning valve on
        newHash = currentHash ^ hash<Name>{}(currentValveName);
        maxFlow = max(maxFlow,
                      recurse(valves, currentValveName, released, newFlowRate,
                              newFlow, minutesRemaining, newHash, memo));
    }

    memo[currentHash] = maxFlow;
    return maxFlow;
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
        Name valveName{match};

        for (auto remainder = match.suffix().first;
             regex_search(remainder, line.cend(), match, namePattern);
             remainder = match.suffix().first) {
            valve.tunnels.push_back({match});
        }

        regex_search(line, match, flowRatePattern);
        valve.flowRate = std::stoi(match.str());

        valves[valveName] = move(valve);
    }

    for (const auto &nameAndValve : valves) {
        cout << "name: " << nameAndValve.first << "\nvalve:\n"
             << nameAndValve.second << '\n';
    }

    unordered_set<Name> released;
    unordered_map<size_t, flow_t> memo;
    flow_t currentFlowRate = 0;
    flow_t currentFlow = 0;
    uint minutesRemaining = 30;

    auto finalFlow =
        recurse(valves, {'A', 'A'}, move(released), currentFlowRate,
                currentFlow, minutesRemaining, 0, memo);

    cout << "final flow: " << finalFlow << '\n';

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}