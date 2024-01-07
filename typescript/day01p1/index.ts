import { open } from 'fs/promises';

const time = performance.now()

const lines = (await open('input', 'r')).readLines()

let max = 0
let curr = 0
for await (const line of lines) {
    if (!line.length) {
        max = Math.max(max, curr)
        curr = 0
    } else {
        curr += parseInt(line)
    }
}

console.log(`max cals: ${max}`)
console.log(`elapsed: ${(performance.now() - time).toPrecision(5)}ms`)