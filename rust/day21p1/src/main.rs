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
const INPUT: &str = include_str!("../input");

type MonkeyId = &'static str;

#[derive(Clone)]
enum Op {
    Mul,
    Add,
    Sub,
    Div,
}

enum Monkey {
    Num(u64),
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

fn recurse(id: MonkeyId, monkeys: &FxHashMap<MonkeyId, Monkey>) -> u64 {
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

    let monkeys = INPUT
        .lines()
        .map(|line| monkey_and_id(line).unwrap().1)
        .collect::<FxHashMap<MonkeyId, Monkey>>();

    println!("root monkey yells: {}", recurse("root", &monkeys));

    println!(
        "elapsed: {} ms",
        time.elapsed().as_micros() as f32 / 1_000.0
    );
}
