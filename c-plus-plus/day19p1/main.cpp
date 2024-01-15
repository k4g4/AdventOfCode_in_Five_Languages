#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

#define INPUT "example"
#define BLUEPRINTS 2
// #define INPUT "input"
// #define BLUEPRINTS 30

#define TOTAL_MINUTES 24

typedef uint Minutes;
typedef uint Ore;
typedef uint Clay;
typedef uint Obsidian;
typedef uint Geode;
typedef uint Robot;

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

    Resources collect() {
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

        return seed;
    }
};

Geode gather(Resources resources, const Blueprint& blueprint, Minutes minutes) {
    if (!minutes) {
        return 0;
    }

    Geode maxGeodes = 0;

    if (resources.ore >= blueprint.oreBot) {
        Resources collected = resources.collect();
        collected.oreBots += 1;
        collected.ore -= blueprint.oreBot;
        maxGeodes = max(maxGeodes, gather(collected, blueprint, minutes - 1));
    }

    if (resources.ore >= blueprint.clayBot) {
        Resources collected = resources.collect();
        collected.clayBots += 1;
        collected.ore -= blueprint.clayBot;
        maxGeodes = max(maxGeodes, gather(collected, blueprint, minutes - 1));
    }

    if (resources.clay >= blueprint.obsidianBot.first &&
        resources.ore >= blueprint.obsidianBot.second) {
        Resources collected = resources.collect();
        collected.obsidianBots += 1;
        collected.clay -= blueprint.obsidianBot.first;
        collected.ore -= blueprint.obsidianBot.second;
        maxGeodes = max(maxGeodes, gather(collected, blueprint, minutes - 1));
    }

    if (resources.obsidian >= blueprint.geodeBot.first &&
        resources.ore >= blueprint.geodeBot.second) {
        Resources collected = resources.collect();
        collected.geodeBots += 1;
        collected.obsidian -= blueprint.geodeBot.first;
        collected.ore -= blueprint.geodeBot.second;
        maxGeodes = max(maxGeodes, gather(collected, blueprint, minutes - 1));
    }

    Ore maxOre = max({blueprint.oreBot, blueprint.clayBot,
                      blueprint.obsidianBot.second, blueprint.geodeBot.second});
    if (resources.ore < maxOre) {
        maxGeodes =
            max(maxGeodes, gather(resources.collect(), blueprint, minutes - 1));
    }

    Geode geodes = resources.geodeBots + maxGeodes;
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

    for (auto blueprint : blueprints) {
        cout << gather(Resources{}, blueprint, TOTAL_MINUTES) << endl;
    }

    cout << "elapsed: "
         << duration_cast<duration<double, milli>>(steady_clock::now() - time)
                .count()
         << " ms" << endl;
}