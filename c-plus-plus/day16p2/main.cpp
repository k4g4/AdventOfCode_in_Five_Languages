#include <array>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>

// #define INPUT "example"
// #define VALVES_LEN 10
#define INPUT "input"
#define VALVES_LEN 51

using namespace std;
using namespace std::chrono;

typedef u_int64_t pressure_t;
typedef u_int8_t index_t;

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
    array<index_t, 5> tunnels;
    u_int8_t tunnelsLen;
};

typedef tuple<index_t, index_t, bitset<VALVES_LEN>, u_int8_t> memokey_t;

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
        HASH_COMBINE(3);
        return seed;
    }
};

pressure_t highestPressure(const array<Valve, VALVES_LEN> &valves,
                           index_t valveIndex0, index_t valveIndex1,
                           index_t prevValveIndex0, index_t prevValveIndex1,
                           bitset<VALVES_LEN> released,
                           const bitset<VALVES_LEN> allReleased,
                           u_int8_t minutesRemaining,
                           unordered_map<memokey_t, pressure_t> &memo) {
    if (minutesRemaining == 0 || released == allReleased) {
        return 0;
    }

    auto minIndex = min(valveIndex0, valveIndex1);
    auto maxIndex = max(valveIndex0, valveIndex1);
    memokey_t memoKey =
        make_tuple(minIndex, maxIndex, released, minutesRemaining);

    auto memory = memo.find(memoKey);
    if (memory != memo.end()) {
        return memory->second;
    }

    pressure_t maxPressure = 0;
    auto valve0 = valves[valveIndex0];
    auto valve1 = valves[valveIndex1];
    auto valve0Releasable = valve0.flowRate && !released[valveIndex0];
    auto valve1Releasable = valve1.flowRate && !released[valveIndex1];

    if (!(valve0Releasable && valve0.flowRate > 15) &&
        !(valve1Releasable && valve1.flowRate > 15)) {
        for (index_t i = 0; i < valve0.tunnelsLen; i++) {
            for (index_t j = 0; j < valve1.tunnelsLen; j++) {
                if (valve0.tunnels[i] != prevValveIndex0 &&
                    valve1.tunnels[j] != prevValveIndex1) {
                    auto pressure = highestPressure(
                        valves, valve0.tunnels[i], valve1.tunnels[j],
                        valveIndex0, valveIndex1, released, allReleased,
                        minutesRemaining - 1, memo);
                    maxPressure = max(maxPressure, pressure);
                }
            }
        }
    }

    if (valve0Releasable) {
        released[valveIndex0] = true;
        for (index_t i = 0; i < valve1.tunnelsLen; i++) {
            if (valve1.tunnels[i] != prevValveIndex1) {
                auto pressureAfterReleasing =
                    highestPressure(valves, valveIndex0, valve1.tunnels[i],
                                    valveIndex0, valveIndex1, released,
                                    allReleased, minutesRemaining - 1, memo) +
                    (valve0.flowRate * (minutesRemaining - 1));
                maxPressure = max(maxPressure, pressureAfterReleasing);
            }
        }
        released[valveIndex0] = false;
    }
    if (valve1Releasable) {
        released[valveIndex1] = true;
        for (index_t i = 0; i < valve0.tunnelsLen; i++) {
            if (valve0.tunnels[i] != prevValveIndex0) {
                auto pressureAfterReleasing =
                    highestPressure(valves, valve0.tunnels[i], valveIndex1,
                                    valveIndex0, valveIndex1, released,
                                    allReleased, minutesRemaining - 1, memo) +
                    (valve1.flowRate * (minutesRemaining - 1));
                maxPressure = max(maxPressure, pressureAfterReleasing);
            }
        }
        released[valveIndex1] = false;
    }
    if (valveIndex0 != valveIndex1 && valve0Releasable && valve1Releasable) {
        released[valveIndex0] = true;
        released[valveIndex1] = true;
        auto pressureAfterReleasing =
            highestPressure(valves, valveIndex0, valveIndex1, valveIndex0,
                            valveIndex1, released, allReleased,
                            minutesRemaining - 1, memo) +
            (valve0.flowRate * (minutesRemaining - 1)) +
            (valve1.flowRate * (minutesRemaining - 1));
        maxPressure = max(maxPressure, pressureAfterReleasing);
    }

    memo[memoKey] = maxPressure;
    return maxPressure;
}

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;

    const regex namePattern{"[A-Z]{2}"};
    const regex flowRatePattern{"\\d+"};
    smatch match;

    array<Valve, VALVES_LEN> valves;
    array<Name, VALVES_LEN> valveNames;
    array<array<Name, 5>, VALVES_LEN> valveTunnelsArr;

    for (index_t i = 0; i < valves.size(); i++) {
        getline(input, line);

        Valve valve;

        regex_search(line, match, namePattern);
        Name valveName{match};

        array<Name, 5> valveTunnels;
        u_int8_t tunnelsLen = 0;
        for (auto suffix = match.suffix().first;
             regex_search(suffix, line.cend(), match, namePattern);
             suffix = match.suffix().first) {
            valveTunnels[tunnelsLen++] = {match};
        }
        valve.tunnelsLen = tunnelsLen;

        regex_search(line, match, flowRatePattern);
        valve.flowRate = stoi(match.str());

        valves[i] = valve;
        valveNames[i] = valveName;
        valveTunnelsArr[i] = valveTunnels;
    }

#define VALVE_INDEX(name) \
    find(valveNames.begin(), valveNames.end(), name) - valveNames.begin()

    for (index_t i = 0; i < valves.size(); i++) {
        for (index_t j = 0; j < valves[i].tunnelsLen; j++) {
            valves[i].tunnels[j] = VALVE_INDEX(valveTunnelsArr[i][j]);
        }
    }

    bitset<VALVES_LEN> allReleased;
    for (int i = 0; i < valves.size(); i++) {
        if (valves[i].flowRate) {
            allReleased[i] = true;
        }
    }

    auto aaName = Name{'A', 'A'};
    index_t start = VALVE_INDEX(aaName);
    bitset<VALVES_LEN> released;
    u_int8_t minutesRemaining = 26;
    unordered_map<memokey_t, pressure_t> memo;

    auto finalPressure =
        highestPressure(valves, start, start, start, start, released,
                        allReleased, minutesRemaining, memo);

    cout << "final pressure: " << finalPressure << '\n';

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}