from typing import Optional
from enum import Enum
from time import perf_counter_ns

INPUT_FILE = 'input'
ROWS, COLUMNS = 200, 85
X_OFFSET = 490
# INPUT_FILE = 'example'
# ROWS, COLUMNS = 10, 10
# X_OFFSET = 494


time = perf_counter_ns()


class Tile(Enum):
    Air = 0,
    Rock = 1,
    Sand = 2,


class Cave:
    def __init__(self):
        self.tiles = [[Tile.Air for _ in range(COLUMNS)] for _ in range(ROWS)]

    def __getitem__(self, ix: tuple[int, int]) -> Tile:
        return self.tiles[ix[1]][ix[0] - X_OFFSET]

    def __setitem__(self, ix: tuple[int, int], tile: Tile):
        self.tiles[ix[1]][ix[0] - X_OFFSET] = tile

    def print(self):
        for row in self.tiles:
            for tile in row:
                match tile:
                    case Tile.Air:
                        print('.', end='')
                    case Tile.Rock:
                        print('#', end='')
                    case Tile.Sand:
                        print('o', end='')
            print()
        print()

    def sand_count(self) -> int:
        return sum(1 for row in self.tiles for tile in row if tile == Tile.Sand)


cave = Cave()

with open(INPUT_FILE) as input_file:
    for line in input_file:
        line = line.strip()

        prev_pos: Optional[tuple[int, int]] = None
        for pos in line.split(' -> '):
            [x, y] = list(map(int, pos.split(',')))

            if prev_pos is not None:
                prev_x, prev_y = prev_pos

                if x == prev_x:
                    small_y, big_y = (y, prev_y) if y < prev_y else (prev_y, y)

                    for mid_y in range(small_y, big_y + 1):
                        cave[x, mid_y] = Tile.Rock
                else:
                    small_x, big_x = (x, prev_x) if x < prev_x else (prev_x, x)

                    for mid_x in range(small_x, big_x + 1):
                        cave[mid_x, y] = Tile.Rock

            prev_pos = (x, y)

try:
    while True:
        x, y = 500, 0
        while True:
            if cave[x, y + 1] == Tile.Air:
                y += 1
            elif cave[x - 1, y + 1] == Tile.Air:
                x -= 1
                y += 1
            elif cave[x + 1, y + 1] == Tile.Air:
                x += 1
                y += 1
            else:
                cave[x, y] = Tile.Sand
                break
except IndexError:
    #cave.print()
    print(cave.sand_count())

print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
