import { open } from 'fs/promises';

const time = performance.now()

const lines = (await open('input')).readLines()
let stacks = [
    [], //so that index starts at 1
    [
        'Q', 'H', 'C', 'T', 'N', 'S', 'V', 'B'
    ],
    [
        'G', 'B', 'D', 'W'
    ],
    [
        'B', 'Q', 'S', 'T', 'R', 'W', 'F'
    ],
    [
        'N', 'D', 'J', 'Z', 'S', 'W', 'G', 'L'
    ],
    [
        'F', 'V', 'D', 'P', 'M'
    ],
    [
        'J', 'W', 'F'
    ],
    [
        'V', 'J', 'B', 'Q', 'N', 'L'
    ],
    [
        'N', 'S', 'Q', 'J', 'C', 'R', 'T', 'G'
    ],
    [
        'M', 'D', 'W', 'C', 'Q', 'S', 'J'
    ]
]

for await (const line of lines) {
    const [num, from, to] = line.split(' ').map(Number)
    stacks[to].splice(0, 0, ...stacks[from].splice(0, num).reverse())
}

let top = ''

for (const stack of stacks) {
    top += stack[0] ?? ''
}

console.log(`top: ${top}`)

console.log(`elapsed: ${(performance.now() - time).toPrecision(5)} ms`)