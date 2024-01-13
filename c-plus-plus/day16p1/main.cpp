#include <array>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>

#define INPUT "example"
#define VALVES_LEN 10
// #define INPUT "input"
// #define VALVES_LEN 51

using namespace std;
using namespace std::chrono;

typedef u_int64_t pressure_t;

struct Name : array<char, 2> {
    Name() {}
    Name(char zero, char one) : array{zero, one} {}
    Name(const smatch &match) {
        (*this)[0] = *match[0].first;
        (*this)[1] = *(match[0].first + 1);
    }
};

struct Valve {
    pressure_t flowRate;
    array<u_int8_t, 5> tunnels;
    u_int8_t tunnelsLen;
};

typedef tuple<u_int8_t, bitset<VALVES_LEN>, u_int8_t> memokey_t;

template <>
struct std::hash<memokey_t> {
    size_t operator()(const memokey_t &memoKey) const noexcept {
        size_t seed = 0;

#define HASH_COMBINE(i)                                                  \
    seed ^= hash<tuple_element<i, memokey_t>::type>{}(get<i>(memoKey)) + \
            0x9e3779b9 + (seed << 6) + (seed >> 2)

        HASH_COMBINE(0);
        HASH_COMBINE(1);
        HASH_COMBINE(2);
        return seed;
    }
};

pressure_t highestPressure(const array<Valve, VALVES_LEN> &valves,
                           u_int8_t currentValve, bitset<VALVES_LEN> released,
                           u_int8_t minutesRemaining,
                           unordered_map<memokey_t, pressure_t> &memo) {
    if (minutesRemaining == 0) {
        return 0;
    }

    memokey_t memoKey = make_tuple(currentValve, released, minutesRemaining);
    auto memory = memo.find(memoKey);
    if (memory != memo.cend()) {
        return memory->second;
    }

    pressure_t maxPressure = 0;
    auto valve = valves[currentValve];

    for (u_int8_t i = 0; i < valve.tunnelsLen; i++) {
        maxPressure =
            max(maxPressure, highestPressure(valves, valve.tunnels[i], released,
                                             minutesRemaining - 1, memo));
    }

    if (!released[currentValve]) {
        released[currentValve] = true;
        auto pressureAfterReleasing =
            highestPressure(valves, currentValve, released,
                            minutesRemaining - 1, memo) +
            (valve.flowRate * (minutesRemaining - 1));
        maxPressure = max(maxPressure, pressureAfterReleasing);
    }

    memo[memoKey] = maxPressure;
    return maxPressure;
}

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;

    regex namePattern{"[A-Z]{2}"};
    regex flowRatePattern{"\\d+"};
    smatch match;

    array<Valve, VALVES_LEN> valves;
    array<Name, VALVES_LEN> valveNames;
    array<array<Name, 5>, VALVES_LEN> valveTunnelsArr;

    for (u_int8_t i = 0; i < VALVES_LEN; i++) {
        getline(input, line);

        Valve valve;

        regex_search(line, match, namePattern);
        Name valveName{match};

        array<Name, 5> valveTunnels;
        u_int8_t tunnelsLen = 0;
        for (auto rem = match.suffix().first;
             regex_search(rem, line.cend(), match, namePattern);
             rem = match.suffix().first) {
            valveTunnels[tunnelsLen++] = {match};
        }
        valve.tunnelsLen = tunnelsLen;

        regex_search(line, match, flowRatePattern);
        valve.flowRate = std::stoi(match.str());

        valves[i] = valve;
        valveNames[i] = valveName;
        valveTunnelsArr[i] = valveTunnels;
    }

#define VALVE_INDEX(name) \
    find(valveNames.begin(), valveNames.end(), name) - valveNames.begin()

    for (u_int8_t i = 0; i < VALVES_LEN; i++) {
        for (u_int8_t j = 0; j < valves[i].tunnelsLen; j++) {
            valves[i].tunnels[j] = VALVE_INDEX(valveTunnelsArr[i][j]);
        }
    }

    bitset<VALVES_LEN> released;
    unordered_map<memokey_t, pressure_t> memo;
    u_int8_t minutesRemaining = 30;
    auto aaName = Name{'A', 'A'};
    u_int8_t currentValve = VALVE_INDEX(aaName);

    auto finalPressure = highestPressure(valves, currentValve, move(released),
                                         minutesRemaining, memo);

    cout << "final pressure: " << finalPressure << '\n';

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}