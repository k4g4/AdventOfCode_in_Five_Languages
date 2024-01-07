import { open } from 'fs/promises';

const time = performance.now()

const lines = (await open('input', 'r')).readLines()

let top_three: number[] = []
let curr = 0
for await (const line of lines) {
    if (!line.length) {
        if (!top_three.length) {
            top_three.push(curr)
        } else {
            const i = top_three.findIndex(val => val < curr)
            if (i >= 0) {
                top_three.splice(i, 0, curr)
                if (top_three.length > 3) {
                    top_three.pop()
                }
            }
        }
        curr = 0
    } else {
        curr += parseInt(line)
    }
}

const total = top_three.reduce((x, y) => x + y)

console.log(`top three cals: ${total}`)
console.log(`elapsed: ${(performance.now() - time).toPrecision(5)}ms`)