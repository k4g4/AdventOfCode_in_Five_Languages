from typing import Optional
from time import perf_counter_ns

time = perf_counter_ns()


type Packet = list[Packet | int]


def compare(left: Packet | int, right: Packet | int) -> Optional[bool]:
    if isinstance(left, int) and isinstance(right, int):
        if left != right:
            return left < right
        else:
            return

    left_norm: Packet
    if isinstance(left, int):
        left_norm = [left]
    elif isinstance(left, list):
        left_norm = left
    else:
        return

    right_norm: Packet
    if isinstance(right, int):
        right_norm = [right]
    elif isinstance(right, list):
        right_norm = right
    else:
        return

    for left_item, right_item in zip(left_norm, right_norm):
        comparison = compare(left_item, right_item)
        if comparison is not None:
            return comparison
        
    if len(left_norm) != len(right_norm):
        return len(left_norm) < len(right_norm)


with open('input') as input_file:
    pairs: list[tuple[Packet, Packet]] = []

    first: Optional[Packet] = None
    for i, line in enumerate(input_file):
        match i % 3:
            case 0:
                first = eval(line)
            case 1 if first is not None:
                second: Packet = eval(line)
                pairs.append((first, second))

    total = 0
    for i, pair in enumerate(pairs, 1):
        if compare(*pair):
            total += i

    print(f'total: {total}')


print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
