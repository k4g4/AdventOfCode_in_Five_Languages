from time import perf_counter_ns

INPUT_FILE = 'input'
# INPUT_FILE = 'example'

time = perf_counter_ns()


with open(INPUT_FILE) as input_file:
    for line in input_file:
        line = line.strip()
        print(line)


print(f'elapsed: {(perf_counter_ns() - time) / 1_000_000} ms')
