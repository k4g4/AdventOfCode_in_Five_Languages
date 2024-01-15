#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

// #define INPUT "example"
// #define BLUEPRINTS 2
#define INPUT "input"
#define BLUEPRINTS 3

#define TOTAL_MINUTES 32

typedef uint8_t Minutes;
typedef uint32_t Ore;
typedef uint32_t Clay;
typedef uint32_t Obsidian;
typedef uint32_t Geode;
typedef uint8_t Robot;

struct Blueprint {
    Ore oreBot;
    Ore clayBot;
    pair<Clay, Ore> obsidianBot;
    pair<Obsidian, Ore> geodeBot;
};

struct Resources {
    Ore ore;
    Clay clay;
    Obsidian obsidian;
    Robot oreBots, clayBots, obsidianBots, geodeBots;

    Resources()
        : ore{0},
          clay{0},
          obsidian{0},
          oreBots{1},
          clayBots{0},
          obsidianBots{0},
          geodeBots{0} {}

    bool operator==(const Resources other) const {
        return ore == other.ore && clay == other.clay &&
               obsidian == other.obsidian && oreBots == other.oreBots &&
               clayBots == other.clayBots &&
               obsidianBots == other.obsidianBots &&
               geodeBots == other.geodeBots;
    }

    Resources collect() const {
        Resources r = *this;
        r.ore += r.oreBots;
        r.clay += r.clayBots;
        r.obsidian += r.obsidianBots;
        return r;
    }
};

template <>
struct std::hash<pair<Resources, Minutes>> {
    size_t operator()(const pair<Resources, Minutes>& key) const noexcept {
        size_t seed;
        auto hashCombine = [&](uint i) {
            seed ^= hash<uint>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        };

        hashCombine(key.first.ore);
        hashCombine(key.first.clay);
        hashCombine(key.first.obsidian);
        hashCombine(key.first.oreBots);
        hashCombine(key.first.clayBots);
        hashCombine(key.first.obsidianBots);
        hashCombine(key.first.geodeBots);
        hashCombine(key.second);

        return seed;
    }
};

typedef unordered_map<pair<Resources, Minutes>, Geode> Memo;

Geode gather(const Resources resources, const Minutes minutes,
             const Blueprint& blueprint, Memo& memo) {
    if (!minutes) {
        return 0;
    }

    auto memory = memo.find(make_pair(resources, minutes));
    if (memory != memo.end()) {
        return memory->second;
    }

    Geode maxGeodes = 0;

    if (resources.obsidian >= blueprint.geodeBot.first &&
        resources.ore >= blueprint.geodeBot.second) {
        Resources collected = resources.collect();
        collected.geodeBots += 1;
        collected.obsidian -= blueprint.geodeBot.first;
        collected.ore -= blueprint.geodeBot.second;
        maxGeodes =
            max(maxGeodes, gather(collected, minutes - 1, blueprint, memo));
    } else {
        if (resources.clay >= blueprint.obsidianBot.first &&
            resources.ore >= blueprint.obsidianBot.second) {
            Resources collected = resources.collect();
            collected.obsidianBots += 1;
            collected.clay -= blueprint.obsidianBot.first;
            collected.ore -= blueprint.obsidianBot.second;
            maxGeodes =
                max(maxGeodes, gather(collected, minutes - 1, blueprint, memo));
        }

        if (resources.ore >= blueprint.clayBot) {
            Resources collected = resources.collect();
            collected.clayBots += 1;
            collected.ore -= blueprint.clayBot;
            maxGeodes =
                max(maxGeodes, gather(collected, minutes - 1, blueprint, memo));
        }

        if (resources.ore >= blueprint.oreBot) {
            Resources collected = resources.collect();
            collected.oreBots += 1;
            collected.ore -= blueprint.oreBot;
            maxGeodes =
                max(maxGeodes, gather(collected, minutes - 1, blueprint, memo));
        }

        Ore maxOre =
            max({blueprint.oreBot, blueprint.clayBot,
                 blueprint.obsidianBot.second, blueprint.geodeBot.second});
        if (resources.ore < maxOre) {
            maxGeodes = max(maxGeodes, gather(resources.collect(), minutes - 1,
                                              blueprint, memo));
        }
    }

    Geode geodes = resources.geodeBots + maxGeodes;
    memo[make_pair(resources, minutes)] = geodes;
    return geodes;
}

int main() {
    auto time = steady_clock::now();

    ifstream input{INPUT};
    string line;
    regex num{"\\d+"};
    smatch match;

    array<Blueprint, BLUEPRINTS> blueprints;
    for (auto& blueprint : blueprints) {
        getline(input, line);
        regex_search(line, match, num);
        auto nextInt = [&](auto& resource) {
            regex_search(match.suffix().first, line.cend(), match, num);
            resource = stoi(match[0].str());
        };
        nextInt(blueprint.oreBot);
        nextInt(blueprint.clayBot);
        nextInt(blueprint.obsidianBot.second);
        nextInt(blueprint.obsidianBot.first);
        nextInt(blueprint.geodeBot.second);
        nextInt(blueprint.geodeBot.first);
    }

    ulong total = 1;
    for (int i = 0; i < BLUEPRINTS; i++) {
        Memo memo;
        total *= gather(Resources{}, TOTAL_MINUTES, blueprints[i], memo);
    }

    cout << "product of max geodes: " << total << endl;

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}