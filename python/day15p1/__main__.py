import re
from typing import Optional
from time import perf_counter_ns
from dataclasses import dataclass

INPUT_FILE = 'input'
X = 3_000_000
Y = 2_000_000
# INPUT_FILE = 'example'
# X = 10
# Y = 10

time = perf_counter_ns()


@dataclass
class Sensor:
    x: int
    y: int
    b_x: int
    b_y: int
    prev_dist: Optional[int] = None
    _b_dist: Optional[int] = None

    @staticmethod
    def _dist(x0: int, y0: int, x1: int, y1: int) -> int:
        return abs(x0 - x1) + abs(y0 - y1)

    def dist(self, x: int, y: int) -> int:
        self.prev_dist = self._dist(self.x, self.y, x, y)
        return self.prev_dist

    @property
    def b_dist(self) -> int:
        if self._b_dist is None:
            self._b_dist = self._dist(self.x, self.y, self.b_x, self.b_y)
        return self._b_dist


sensors: list[Sensor] = []

with open(INPUT_FILE) as input_file:
    pos = r'x=(-?\d+), y=(-?\d+)'
    pattern = re.compile(f'Sensor at {pos}: closest beacon is at {pos}\n?')

    for line in input_file:
        if match := pattern.search(line):
            sensors.append(Sensor(*map(int, match.groups())))

positions: set[int] = set()
x = X
for sensor in sensors:
    sensor.dist(x, Y)

for direction in [1, -1]:
    while True:
        x += direction
        inside = False
        leaving = True

        for sensor in sensors:
            if prev_dist := sensor.prev_dist:
                dist = sensor.dist(x, Y)

                if dist <= prev_dist:
                    leaving = False

                if dist <= sensor.b_dist:
                    positions.add(x)
                    inside = True

        if not inside and leaving:
            break

    x = X + 1

for sensor in sensors:
    if sensor.y == Y and sensor.x in positions:
        positions.remove(sensor.x)

    if sensor.b_y == Y and sensor.b_x in positions:
        positions.remove(sensor.b_x)


print(f'positions: {len(positions)}')


print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
