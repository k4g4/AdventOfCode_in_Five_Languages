use rustc_hash::FxHashSet;
use std::{array::from_fn, iter::once, mem::swap, time::Instant};

// const INPUT: &str = include_str!("../example");
// const WIDTH: usize = 8;
// const HEIGHT: usize = 6;
const INPUT: &str = include_str!("../input");
const WIDTH: usize = 122;
const HEIGHT: usize = 27;

fn index(i: usize, j: usize) -> usize {
    i * WIDTH + j
}

enum Blizzard {
    North,
    South,
    East,
    West,
}

impl TryFrom<char> for Blizzard {
    type Error = ();

    fn try_from(value: char) -> Result<Self, Self::Error> {
        match value {
            '^' => Ok(Self::North),
            'v' => Ok(Self::South),
            '>' => Ok(Self::East),
            '<' => Ok(Self::West),
            _ => Err(()),
        }
    }
}

#[derive(Clone, Copy, PartialEq, Eq, Hash)]
enum ProbeState {
    GoingToEnd,
    BackToStart,
    BackToEnd,
}

fn main() {
    let time = Instant::now();

    let mut blizzards: [Vec<Blizzard>; WIDTH * HEIGHT] = {
        let mut iter = INPUT.lines().flat_map(|line| {
            line.chars()
                .map(|tile| tile.try_into().into_iter().collect())
        });
        from_fn(|_| iter.next().unwrap())
    };
    let mut new_blizzards = from_fn(|_| vec![]);

    let mut probes = FxHashSet::default();
    probes.insert((0, 1, ProbeState::GoingToEnd));
    let mut new_probes = FxHashSet::default();

    'minutes: for minute in 1.. {
        for i in 1..HEIGHT - 1 {
            for j in 1..WIDTH - 1 {
                for blizzard in blizzards[index(i, j)].drain(..) {
                    let (i, j) = match blizzard {
                        Blizzard::North => (i - 1, j),
                        Blizzard::South => (i + 1, j),
                        Blizzard::East => (i, j + 1),
                        Blizzard::West => (i, j - 1),
                    };
                    let i = if i == 0 {
                        HEIGHT - 2
                    } else if i == HEIGHT - 1 {
                        1
                    } else {
                        i
                    };
                    let j = if j == 0 {
                        WIDTH - 2
                    } else if j == WIDTH - 1 {
                        1
                    } else {
                        j
                    };
                    new_blizzards[index(i, j)].push(blizzard);
                }
            }
        }
        swap(&mut blizzards, &mut new_blizzards);

        for (i, j, state) in probes.drain() {
            let offsets = if i == 0 {
                &[(1, 0)]
            } else if i == HEIGHT - 1 {
                &[(-1, 0)]
            } else if i == HEIGHT - 2 && j == 1 {
                &[(-1, 0), (0, 1)]
            } else if i == 1 && j == WIDTH - 2 {
                &[(0, -1), (1, 0)]
            } else if j == 1 {
                &[(-1, 0), (1, 0), (0, 1)]
            } else if i == 1 {
                &[(0, -1), (0, 1), (1, 0)]
            } else if j == WIDTH - 2 {
                &[(-1, 0), (1, 0), (0, -1)]
            } else if i == HEIGHT - 2 {
                &[(0, -1), (0, 1), (-1, 0)]
            } else {
                &[(-1, 0), (1, 0), (0, -1), (0, 1)] as &[(i32, i32)]
            };

            for (offset_i, offset_j) in offsets.iter().chain(once(&(0, 0))) {
                let (i, j) = (
                    (i as i32 + offset_i) as usize,
                    (j as i32 + offset_j) as usize,
                );

                if blizzards[index(i, j)].is_empty() {
                    let state = match state {
                        ProbeState::GoingToEnd if i == HEIGHT - 1 => ProbeState::BackToStart,
                        ProbeState::BackToStart if i == 0 => ProbeState::BackToEnd,
                        ProbeState::BackToEnd if i == HEIGHT - 1 => {
                            println!("minutes: {minute}");
                            break 'minutes;
                        }
                        state => state,
                    };
                    new_probes.insert((i, j, state));
                }
            }
        }
        swap(&mut probes, &mut new_probes);
    }

    println!(
        "elapsed: {} ms",
        time.elapsed().as_micros() as f32 / 1_000.0
    );
}
