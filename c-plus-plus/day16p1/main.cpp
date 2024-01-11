#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
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

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;

    const regex namePattern{"[A-Z]{2}"};
    const regex flowRatePattern{"\\d+"};
    smatch match;

    map<Name, Valve> valves;

    while (getline(input, line)) {
        Valve valve;

        regex_search(line, match, namePattern);
        Name name(match);

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

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}