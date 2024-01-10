import re
from typing import Generator
from time import perf_counter_ns
from dataclasses import dataclass

time = perf_counter_ns()

INPUT_FILE = 'input'
WINDOW = 4_000_000
# INPUT_FILE = 'example'
# WINDOW = 20

def dist(x0: int, y0: int, x1: int, y1: int) -> int:
    return abs(x0 - x1) + abs(y0 - y1)

@dataclass
class Sensor:
    x: int
    y: int
    b_dist: int

    def within(self, x: int, y: int) -> bool:
        return dist(self.x, self.y, x, y) <= self.b_dist

    def boundary(self) -> Generator[tuple[int, int], None, None]:
        radius = self.b_dist + 1
        north = self.y - radius
        south = self.y + radius
        west = self.x - radius
        east = self.x + radius

        # west to north
        for i in range(radius):
            yield west + i, self.y - i

        # north to east
        for i in range(radius):
            yield self.x + i,  north + i
        
        # east to south
        for i in range(radius):
            yield east - i, self.y + i
        
        # south to west
        for i in range(radius):
            yield self.x - i, south - i


sensors: list[Sensor] = []

with open(INPUT_FILE) as input_file:
    pattern = re.compile(r'-?\d+')

    for line in input_file:
        x, y, b_x, b_y = map(lambda match: int(match[0]), pattern.finditer(line))
        b_dist = dist(x, y, b_x, b_y)
        sensors.append(Sensor(x, y, b_dist))

for x, y in (pos for sensor in sensors for pos in sensor.boundary()):
    if 0 <= x <= WINDOW and 0 <= y <= WINDOW:
        if not any(sensor.within(x, y) for sensor in sensors):
            print(f'distress tuning freq: {x * 4_000_000 + y}')
            break

print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
