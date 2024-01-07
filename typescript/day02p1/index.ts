import { open } from 'fs/promises';

const time = performance.now()

enum Hand {
    Rock,
    Paper,
    Scissors,
}

function round(theirHand: Hand, myHand: Hand): number {
    let score = {
        [Hand.Rock]: 1,
        [Hand.Paper]: 2,
        [Hand.Scissors]: 3,
    }[myHand]

    if ((theirHand === Hand.Rock && myHand === Hand.Paper) ||
        (theirHand === Hand.Paper && myHand === Hand.Scissors) ||
        (theirHand === Hand.Scissors && myHand === Hand.Rock)) {
        score += 6
    } else if (theirHand === myHand) {
        score += 3
    }

    return score
}

const lines = (await open('input')).readLines()
let score = 0

for await (const line of lines) {
    const [left, right] = line.split(' ')
    const theirHand = {
        'A': Hand.Rock,
        'B': Hand.Paper,
        'C': Hand.Scissors,
    }[left] ?? Hand.Rock
    const myHand = {
        'X': Hand.Rock,
        'Y': Hand.Paper,
        'Z': Hand.Scissors,
    }[right] ?? Hand.Rock

    score += round(theirHand, myHand)
}

console.log(`max score: ${score}`)

console.log(`elapsed: ${(performance.now() - time).toPrecision(5)} ms`)