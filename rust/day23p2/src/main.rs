use rustc_hash::{FxHashMap, FxHashSet};
use std::time::Instant;

// const INPUT: &str = include_str!("../example");
const INPUT: &str = include_str!("../input");

#[derive(Clone, Copy, Debug)]
enum Dir {
    North,
    South,
    West,
    East,
}
use Dir::*;

fn main() {
    let time = Instant::now();

    let mut elves = INPUT
        .lines()
        .enumerate()
        .flat_map(|(i, line)| line.chars().map(move |c| (i, c)).enumerate())
        .filter_map(|(j, (i, c))| (c == '#').then(|| (i as isize, j as isize)))
        .collect::<FxHashSet<_>>();

    let mut new_elves = FxHashMap::<(isize, isize), [Option<(isize, isize)>; 2]>::default();

    let mut dirs = [North, South, West, East];

    for round in 1.. {
        'elves: for &(i, j) in &elves {
            let offsets = [
                (-1, -1),
                (-1, 0),
                (-1, 1),
                (0, -1),
                (0, 1),
                (1, -1),
                (1, 0),
                (1, 1),
            ];
            let elves_contains =
                |&(off_i, off_j): &(isize, isize)| elves.contains(&(i + off_i, j + off_j));

            if offsets.iter().any(elves_contains) {
                for dir in dirs {
                    let offsets = match dir {
                        North => [(-1, -1), (-1, 0), (-1, 1)],
                        South => [(1, -1), (1, 0), (1, 1)],
                        West => [(-1, -1), (0, -1), (1, -1)],
                        East => [(-1, 1), (0, 1), (1, 1)],
                    };
                    if !offsets.iter().any(elves_contains) {
                        let (off_i, off_j) = offsets[1];
                        let [first, second] = new_elves.entry((i + off_i, j + off_j)).or_default();
                        *if first.is_none() { first } else { second } = Some((i, j));
                        continue 'elves;
                    }
                }
            }

            new_elves.entry((i, j)).or_default();
        }
        elves.clear();

        let mut elf_moved = false;
        for (new_elf, [first, second]) in new_elves.drain() {
            if let Some(second) = second {
                let first = first.unwrap();
                elves.insert(first);
                elves.insert(second);
            } else {
                if first.is_some() {
                    elf_moved = true;
                }
                elves.insert(new_elf);
            }
        }

        if !elf_moved {
            println!("no elf moved at round: {round}");
            break;
        }

        dirs.rotate_left(1);
    }

    println!(
        "elapsed: {} ms",
        time.elapsed().as_micros() as f32 / 1_000.0
    );
}
