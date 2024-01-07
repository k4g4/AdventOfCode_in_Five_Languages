import { open } from 'fs/promises';

const time = performance.now()

enum Hand {
    Rock,
    Paper,
    Scissors,
}

enum Goal {
    Lose,
    Draw,
    Win,
}

function round(theirHand: Hand, goal: Goal): number {
    let score = {
        [Goal.Lose]: 0,
        [Goal.Draw]: 3,
        [Goal.Win]: 6,
    }[goal]

    const [rockPoints, paperPoints, scissorPoints] = [1, 2, 3]

    switch (goal) {
        case Goal.Lose: {
            switch (theirHand) {
                case Hand.Rock: {
                    score += scissorPoints
                    break
                }
                case Hand.Paper: {
                    score += rockPoints
                    break
                }
                case Hand.Scissors: {
                    score += paperPoints
                    break
                }
            }
            break
        }

        case Goal.Draw: {
            switch (theirHand) {
                case Hand.Rock: {
                    score += rockPoints
                    break
                }
                case Hand.Paper: {
                    score += paperPoints
                    break
                }
                case Hand.Scissors: {
                    score += scissorPoints
                    break
                }
            }
            break
        }

        case Goal.Win: {
            switch (theirHand) {
                case Hand.Rock: {
                    score += paperPoints
                    break
                }
                case Hand.Paper: {
                    score += scissorPoints
                    break
                }
                case Hand.Scissors: {
                    score += rockPoints
                    break
                }
            }
            break
        }
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
    const goal = {
        'X': Goal.Lose,
        'Y': Goal.Draw,
        'Z': Goal.Win,
    }[right] ?? Goal.Lose

    score += round(theirHand, goal)
}

console.log(`max score: ${score}`)

console.log(`elapsed: ${(performance.now() - time).toPrecision(5)} ms`)