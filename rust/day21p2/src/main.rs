use nom::{
    branch::alt,
    bytes::complete::tag,
    character::complete::{alpha1, digit1},
    combinator::{map, value},
    sequence::{separated_pair, tuple},
    IResult,
};
use rustc_hash::FxHashMap;
use std::time::Instant;

// const INPUT: &str = include_str!("../example");
// const ROOT_LEFT: &str = "pppw";
// const ROOT_RIGHT: &str = "sjmn";
const INPUT: &str = include_str!("../input");
const ROOT_LEFT: &str = "jhpn";
const ROOT_RIGHT: &str = "jmsg";

const ME: &str = "humn";

type MonkeyId = &'static str;

#[derive(Clone)]
enum Op {
    Mul,
    Add,
    Sub,
    Div,
}

enum Monkey {
    Num(f64),
    Op(MonkeyId, Op, MonkeyId),
}

fn op(input: &'static str) -> IResult<&'static str, Op> {
    alt((
        value(Op::Mul, tag(" * ")),
        value(Op::Add, tag(" + ")),
        value(Op::Sub, tag(" - ")),
        value(Op::Div, tag(" / ")),
    ))(input)
}

fn monkey(input: &'static str) -> IResult<&'static str, Monkey> {
    alt((
        map(digit1, |num: &str| Monkey::Num(num.parse().unwrap())),
        map(tuple((alpha1, op, alpha1)), |(lhs, op, rhs)| {
            Monkey::Op(lhs, op, rhs)
        }),
    ))(input)
}

fn monkey_and_id(input: &'static str) -> IResult<&'static str, (MonkeyId, Monkey)> {
    separated_pair(alpha1, tag(": "), monkey)(input)
}

fn recurse(id: MonkeyId, monkeys: &FxHashMap<MonkeyId, Monkey>) -> f64 {
    match &monkeys[id] {
        Monkey::Num(num) => *num,
        Monkey::Op(lhs, op, rhs) => {
            let (lhs, rhs) = (recurse(lhs, monkeys), recurse(rhs, monkeys));
            match op {
                Op::Mul => lhs * rhs,
                Op::Add => lhs + rhs,
                Op::Sub => lhs - rhs,
                Op::Div => lhs / rhs,
            }
        }
    }
}

fn main() {
    let time = Instant::now();

    let mut monkeys = INPUT
        .lines()
        .map(|line| monkey_and_id(line).unwrap().1)
        .collect::<FxHashMap<MonkeyId, Monkey>>();

    let right = recurse(ROOT_RIGHT, &monkeys);
    let (mut min, mut max) = (4000.0, 5_000_000_000_000.0);
    let my_number = loop {
        *monkeys.get_mut(ME).unwrap() = Monkey::Num(min as f64);
        let min_left = recurse(ROOT_LEFT, &monkeys);
        let min_dist = (min_left - right).abs();
        if min_dist < 0.1 {
            break min;
        }

        *monkeys.get_mut(ME).unwrap() = Monkey::Num(max as f64);
        let max_left = recurse(ROOT_LEFT, &monkeys);
        let max_dist = (max_left - right).abs();
        if max_dist == 0.1 {
            break max;
        }

        let mid = min + (max - min) / 2.0;

        use std::cmp::Ordering::*;
        match min_dist.partial_cmp(&max_dist).unwrap() {
            Less => {
                max = mid;
            }
            Equal => {
                min = mid - 1.0;
                max = mid + 1.0;
            }
            Greater => {
                min = mid;
            }
        }
    };

    println!("my number: {my_number}");

    println!(
        "elapsed: {} ms",
        time.elapsed().as_micros() as f32 / 1_000.0
    );
}
