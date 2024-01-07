import { open } from 'fs/promises';

const time = performance.now()

const lines = (await open('input')).readLines()

function priority(item: string): number {
    const az = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
    return 1 + az.indexOf(item)
}

let total = 0
for await (const line of lines) {
    const left = line.substring(0, line.length / 2)
    const right = line.substring(line.length / 2)
    const leftSet = new Set(left)

    for (const rightItem of right) {
        if (leftSet.has(rightItem)) {
            total += priority(rightItem)
            break
        }
    }
}

console.log(`total: ${total}`)

console.log(`elapsed: ${(performance.now() - time).toPrecision(5)} ms`)