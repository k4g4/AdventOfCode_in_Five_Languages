import { open } from 'fs/promises';

const time = performance.now()

const lines = (await open('input')).readLines()

let total = 0
for await (const line of lines) {
    const [first, second] = line.split(',')
    const [firstFrom, firstTo] = first.split('-').map(Number)
    const [secondFrom, secondTo] = second.split('-').map(Number)

    if ((firstFrom >= secondFrom && firstFrom <= secondTo) || // firstFrom inside
        (secondFrom >= firstFrom && secondFrom <= firstTo) || // secondFrom inside
        (firstTo >= secondFrom && firstTo <= secondTo) || // firstTo inside
        (secondTo >= firstFrom && secondTo <= firstTo)) // secondTo inside
    {
        total++
    }
}

console.log(`total: ${total}`)

console.log(`elapsed: ${(performance.now() - time).toPrecision(5)} ms`)