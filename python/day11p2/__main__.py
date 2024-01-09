from time import perf_counter_ns
from typing import Callable, NamedTuple, Iterable
from functools import partial, reduce

time = perf_counter_ns()

Throw = NamedTuple('Throw', [('item', int), ('monkey_index', int)])

class Monkey:
    def __init__(self, items: list[int], operation: Callable[[int], int],
                 divisible_test: int, if_true: int, if_false: int):
        self.items = items
        self.operation = operation
        self.divisible_test = divisible_test
        self.if_true = if_true
        self.if_false = if_false
        self.times_inspected = 0

    def throws(self) -> list[Throw]:
        throws = []

        for item in self.items:
            item = self.operation(item)
            passed_test = item % self.divisible_test == 0
            throws.append(Throw(item, self.if_true if passed_test else self.if_false))
            self.times_inspected += 1

        self.items.clear()
        return throws


def parse_monkeys(lines: Iterable[str]) -> list[Monkey]:
    monkeys = []

    props = {
        'items': [],
        'operation': lambda _: _,
        'divisible_test': -1,
        'if_true': -1,
        'if_false': -1,
    }

    for line in lines:
        if not line:
            monkeys.append(Monkey(**props))
            continue

        [prop, value] = line.split(':')

        match prop:
            case 'Starting items':
                props['items'] = [int(item.strip()) for item in value.split(',')]

            case 'Operation':
                eval_str = value.removeprefix(' new = ')
                props['operation'] = partial(lambda eval_str, old: eval(eval_str), eval_str)

            case 'Test':
                props['divisible_test'] = int(value.removeprefix(' divisible by '))

            case 'If true':
                props['if_true'] = int(value.removeprefix(' throw to monkey '))

            case 'If false':
                props['if_false'] = int(value.removeprefix(' throw to monkey '))

    return monkeys


with open('input') as input_file:
    monkeys = parse_monkeys(map(str.strip, input_file))

    multiple = reduce(lambda acc, x: acc * x, (monkey.divisible_test for monkey in monkeys))

    for _ in range(10_000):
        for monkey in monkeys:
            for throw in monkey.throws():
                monkeys[throw.monkey_index].items.append(throw.item % multiple)

    monkeys.sort(key=lambda monkey: monkey.times_inspected, reverse=True)

    print(f'monkey business: {monkeys[0].times_inspected * monkeys[1].times_inspected}')


print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
