from time import perf_counter_ns
from typing import Iterable, Optional
from string import ascii_lowercase
from dataclasses import dataclass

time = perf_counter_ns()

START, END = -1, 26
MAX_DIST = 1_000

@dataclass
class Tile:
    height: int
    dist: int = MAX_DIST

def tile_from_char(c: str) -> Tile:
    try:
        return Tile(ascii_lowercase.index(c))
    except ValueError as e:
        match c:
            case "S":
                return Tile(START)
            case "E":
                return Tile(END)
        raise e

def parse_tiles(lines: Iterable[str]) -> list[list[Tile]]:
    return [[tile_from_char(c) for c in line] for line in lines]

with open('example') as input_file:
    tiles = parse_tiles(map(lambda line: line.strip(), input_file))

    type Probe = tuple[int, int]
    probes: list[Probe] = []
    new_probes: list[Probe] = []
    for i, row in enumerate(tiles):
        for j, tile in enumerate(row):
            if tile.height == START:
                probes.append((i, j))
                tile.dist = 0

    offsets: list[Probe] = [(-1, 0), (1, 0), (0, -1), (0, 1)]
    result: int = MAX_DIST
    while probes:
        for probe in probes:
            tile = tiles[probe[0]][probe[1]]

            for offset in offsets:
                i, j = probe[0] + offset[0], probe[1] + offset[1]

                if 0 <= i < len(tiles) and 0 <= j < len(tiles[0]):
                    neighbor = tiles[i][j]

                    if neighbor.dist > tile.dist + 1 and neighbor.height <= tile.height + 1:
                        neighbor.dist = tile.dist + (1 if neighbor.height > START + 1 else 0)
                        new_probes.append((i, j))

                        if neighbor.height == END:
                            result = min(result, neighbor.dist)

        probes, new_probes = new_probes, probes
        new_probes.clear()

    print(f'from closest a to E: {result}')

print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
