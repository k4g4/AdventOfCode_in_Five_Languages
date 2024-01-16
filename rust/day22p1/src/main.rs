use nom::{
    branch::alt,
    bytes::complete::tag,
    character::complete::digit1,
    combinator::{iterator, map, value},
    IResult,
};
use std::time::Instant;

// const INPUT: &str = include_str!("../example");
// const ROWS: usize = 12;
// const COLUMNS: usize = 16;
const INPUT: &str = include_str!("../input");
const ROWS: usize = 200;
const COLUMNS: usize = 150;

enum Tile {
    Space,
    Ground,
    Wall,
}

#[derive(Clone)]
enum Move {
    Left,
    Right,
    Forward(u32),
}

#[derive(Debug)]
enum Dir {
    North,
    South,
    East,
    West,
}

impl Dir {
    fn left(&mut self) {
        *self = match self {
            Dir::North => Dir::West,
            Dir::South => Dir::East,
            Dir::East => Dir::North,
            Dir::West => Dir::South,
        };
    }

    fn right(&mut self) {
        *self = match self {
            Dir::North => Dir::East,
            Dir::South => Dir::West,
            Dir::East => Dir::South,
            Dir::West => Dir::North,
        };
    }

    fn step(&self, (row, column): (usize, usize)) -> (usize, usize) {
        match self {
            Dir::North => ((row + ROWS - 1) % ROWS, column),
            Dir::South => ((row + 1) % ROWS, column),
            Dir::East => (row, (column + 1) % COLUMNS),
            Dir::West => (row, (column + COLUMNS - 1) % COLUMNS),
        }
    }
}

fn next_move(input: &'static str) -> IResult<&'static str, Move> {
    alt((
        value(Move::Left, tag("L")),
        value(Move::Right, tag("R")),
        map(digit1, |steps: &str| Move::Forward(steps.parse().unwrap())),
    ))(input)
}

fn main() {
    let time = Instant::now();

    let mut lines = INPUT.lines();
    let map: [[Tile; COLUMNS]; ROWS] = std::array::from_fn(|_| {
        let mut chars = lines.next().unwrap().chars();
        std::array::from_fn(|_| match chars.next() {
            None | Some(' ') => Tile::Space,
            Some('.') => Tile::Ground,
            Some('#') => Tile::Wall,
            _ => unreachable!(),
        })
    });
    lines.next();

    let mut pos = (
        0,
        map[0]
            .iter()
            .position(|tile| matches!(tile, Tile::Ground))
            .unwrap(),
    );
    let mut dir = Dir::East;

    let mut moves = iterator(lines.next().unwrap(), next_move);
    for m in &mut moves {
        match m {
            Move::Left => {
                dir.left();
            }
            Move::Right => {
                dir.right();
            }
            Move::Forward(steps) => {
                'steps: for _ in 0..steps {
                    let (i, j) = dir.step(pos);
                    match map[i][j] {
                        Tile::Ground => {
                            pos = (i, j);
                        }
                        Tile::Wall => {
                            break;
                        }
                        Tile::Space => match dir {
                            Dir::North => {
                                for (k, row) in map.iter().rev().enumerate() {
                                    match row[j] {
                                        Tile::Space => {}
                                        Tile::Ground => {
                                            pos = (ROWS - 1 - k, j);
                                            break;
                                        }
                                        Tile::Wall => {
                                            break 'steps;
                                        }
                                    }
                                }
                            }
                            Dir::South => {
                                for (k, row) in map.iter().enumerate() {
                                    match row[j] {
                                        Tile::Space => {}
                                        Tile::Ground => {
                                            pos = (k, j);
                                            break;
                                        }
                                        Tile::Wall => {
                                            break 'steps;
                                        }
                                    }
                                }
                            }
                            Dir::East => {
                                for (k, tile) in map[i].iter().enumerate() {
                                    match tile {
                                        Tile::Space => {}
                                        Tile::Ground => {
                                            pos = (i, k);
                                            break;
                                        }
                                        Tile::Wall => {
                                            break 'steps;
                                        }
                                    }
                                }
                            }
                            Dir::West => {
                                for (k, tile) in map[i].iter().rev().enumerate() {
                                    match tile {
                                        Tile::Space => {}
                                        Tile::Ground => {
                                            pos = (i, COLUMNS - 1 - k);
                                            break;
                                        }
                                        Tile::Wall => {
                                            break 'steps;
                                        }
                                    }
                                }
                            }
                        },
                    }
                }
            }
        }
    }

    println!(
        "password: {}",
        (pos.0 + 1) * 1000
            + (pos.1 + 1) * 4
            + match dir {
                Dir::East => 0,
                Dir::South => 1,
                Dir::West => 2,
                Dir::North => 3,
            }
    );

    println!(
        "elapsed: {} ms",
        time.elapsed().as_micros() as f32 / 1_000.0
    );
}
