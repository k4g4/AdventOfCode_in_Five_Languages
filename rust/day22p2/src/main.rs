use nom::{
    branch::alt,
    bytes::complete::tag,
    character::complete::digit1,
    combinator::{iterator, map, value},
    IResult,
};
use phf::{phf_map, Map};
use std::{array::from_fn, time::Instant};

const INPUT: &str = include_str!("../input");
const ROWS: usize = 200;
const COLUMNS: usize = 150;
const SIDE_LEN: usize = 50;
const FACES: usize = 6;

#[derive(Clone, Copy)]
enum Tile {
    Ground,
    Wall,
}
use Tile::*;

#[derive(Clone)]
enum Move {
    Left,
    Right,
    Forward(u32),
}
use Move::*;

#[derive(Clone, Copy)]
enum Dir {
    North,
    South,
    East,
    West,
}
use Dir::*;

impl Dir {
    fn turn_left(&mut self) {
        *self = match self {
            North => West,
            South => East,
            East => North,
            West => South,
        };
    }

    fn turn_right(&mut self) {
        *self = match self {
            North => East,
            South => West,
            East => South,
            West => North,
        };
    }
}

static FACES_MAP: Map<u8, Map<char, (usize, Dir)>> = phf_map! {
    0u8 => phf_map!{
        'N' => (5, North),
        'S' => (2, West),
        'E' => (3, West),
        'W' => (1, West),
    },
    1u8 => phf_map!{
        'N' => (5, East),
        'S' => (2, South),
        'E' => (0, East),
        'W' => (4, East),
    },
    2u8 => phf_map!{
        'N' => (1, North),
        'S' => (3, South),
        'E' => (0, North),
        'W' => (4, South),
    },
    3u8 => phf_map!{
        'N' => (2, North),
        'S' => (5, West),
        'E' => (0, West),
        'W' => (4, West),
    },
    4u8 => phf_map!{
        'N' => (2, East),
        'S' => (5, South),
        'E' => (3, East),
        'W' => (1, East),
    },
    5u8 => phf_map!{
        'N' => (4, North),
        'S' => (0, South),
        'E' => (3, North),
        'W' => (1, South),
    },
};

fn step((face, row, column): (usize, usize, usize), dir: Dir) -> ((usize, usize, usize), Dir) {
    let (row, column) = (row as isize, column as isize);
    let (row, column) = match dir {
        North => (row - 1, column),
        South => (row + 1, column),
        East => (row, column + 1),
        West => (row, column - 1),
    };

    if !(0..50).contains(&row) || !(0..50).contains(&column) {
        let (new_face, new_dir) = FACES_MAP[&(face as u8)][&match dir {
            North => 'N',
            South => 'S',
            East => 'E',
            West => 'W',
        }];

        let (row, column) = match (dir, new_dir) {
            (North, North) | (South, South) | (East, East) | (West, West) => (
                (row + SIDE_LEN as isize) as usize % SIDE_LEN,
                (column + SIDE_LEN as isize) as usize % SIDE_LEN,
            ),
            (East, West) => (SIDE_LEN - 1 - row as usize, SIDE_LEN - 1),
            (West, East) => (SIDE_LEN - 1 - row as usize, 0),
            (North, East) => (column as usize, 0),
            (South, West) => (column as usize, SIDE_LEN - 1),
            (East, North) => (SIDE_LEN - 1, row as usize),
            (West, South) => (0, row as usize),
            _ => unreachable!(),
        };

        ((new_face, row, column), new_dir)
    } else {
        ((face, row as usize, column as usize), dir)
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
    let map: [[[Tile; SIDE_LEN]; SIDE_LEN]; FACES] = {
        let map: [[Tile; COLUMNS]; ROWS] = from_fn(|_| {
            let mut chars = lines.next().unwrap().chars();
            from_fn(|_| match chars.next() {
                Some('.') => Tile::Ground,
                _ => Tile::Wall,
            })
        });
        let face = |i: usize, j: usize| -> [[Tile; SIDE_LEN]; SIDE_LEN] {
            from_fn(|row| from_fn(|column| map[row + i][column + j]))
        };
        [
            face(0, 100),
            face(0, 50),
            face(50, 50),
            face(100, 50),
            face(100, 0),
            face(150, 0),
        ]
    };
    lines.next();

    let mut pos = (1, 0, 0);
    let mut dir = Dir::East;

    let mut moves = iterator(lines.next().unwrap(), next_move);
    for m in &mut moves {
        match m {
            Left => dir.turn_left(),
            Right => dir.turn_right(),
            Forward(steps) => {
                for _ in 0..steps {
                    let ((face, i, j), new_dir) = step(pos, dir);
                    match map[face][i][j] {
                        Ground => (pos, dir) = ((face, i, j), new_dir),
                        Wall => break,
                    }
                }
            }
        }
    }

    let (face, row, column) = pos;
    let (i, j) = match face {
        0 => (0, 100),
        1 => (0, 50),
        2 => (50, 50),
        3 => (100, 50),
        4 => (100, 0),
        5 => (150, 0),
        _ => unreachable!(),
    };
    let password = (i + row + 1) * 1000
        + (j + column + 1) * 4
        + match dir {
            East => 0,
            South => 1,
            West => 2,
            North => 3,
        };

    println!("password: {password}");

    println!(
        "elapsed: {} ms",
        time.elapsed().as_micros() as f32 / 1_000.0
    );
}
