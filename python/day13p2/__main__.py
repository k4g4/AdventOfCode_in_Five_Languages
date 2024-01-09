from typing import Optional
from time import perf_counter_ns
from functools import cmp_to_key

time = perf_counter_ns()


type Packet = list[Packet | int]


def compare(left: Packet | int, right: Packet | int) -> int:
    if isinstance(left, int) and isinstance(right, int):
        if left != right:
            return -1 if left < right else 1
        else:
            return 0

    left_norm: Packet
    if isinstance(left, int):
        left_norm = [left]
    elif isinstance(left, list):
        left_norm = left
    else:
        return 0

    right_norm: Packet
    if isinstance(right, int):
        right_norm = [right]
    elif isinstance(right, list):
        right_norm = right
    else:
        return 0

    for left_item, right_item in zip(left_norm, right_norm):
        comparison: int = compare(left_item, right_item)
        if comparison != 0:
            return comparison

    if len(left_norm) != len(right_norm):
        return -1 if len(left_norm) < len(right_norm) else 1
    
    return 0


with open('input') as input_file:
    packets: list[Packet] = []

    for line in input_file:
        line = line.strip()
        if line:
            packets.append(eval(line))

    packets.append([[2]])
    packets.append([[6]])

    packets.sort(key=cmp_to_key(compare))

    print(f'decoder key: {(packets.index([[2]]) + 1) * (packets.index([[6]]) + 1)}')


print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
