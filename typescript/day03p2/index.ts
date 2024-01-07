import { open } from 'fs/promises';

const time = performance.now()

const lines = (await open('input')).readLines()

function priority(item: string): number {
    const az = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
    return 1 + az.indexOf(item)
}

let total = 0
let i = 0
let sets: Set<string>[] = [new Set(), new Set(), new Set()]
for await (const line of lines) {
    sets[i++ % 3] = new Set(line)
    if (i % 3 == 0) {
        for (const item of sets[0]) {
            if (sets[1].has(item) && sets[2].has(item)) {
                total += priority(item)
                break
            }
        }
    }
}

console.log(`total: ${total}`)

console.log(`elapsed: ${(performance.now() - time).toPrecision(5)} ms`)