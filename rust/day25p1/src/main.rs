use std::time::Instant;

// const INPUT: &str = include_str!("../example");
const INPUT: &str = include_str!("../input");

fn from_snafu(snafu: &str) -> i64 {
    snafu
        .chars()
        .rev()
        .enumerate()
        .map(|(i, c)| {
            5i64.pow(i as u32)
                * match c {
                    '=' => -2,
                    '-' => -1,
                    '0' => 0,
                    '1' => 1,
                    _ => 2,
                }
        })
        .sum()
}

fn to_snafu(num: i64) -> String {
    let mut snafu = vec!['1'];
    let mut n = 1;
    let mut pos = 0;

    while n < num {
        n *= 5;
        pos += 1;
        *snafu.last_mut().unwrap() = '0';
        snafu.push('1');
    }
    snafu.push('0');

    fn increment(digits: &mut [char]) {
        let mut carry;
        for digit in digits {
            (carry, *digit) = match *digit {
                '=' => (false, '-'),
                '-' => (false, '0'),
                '0' => (false, '1'),
                '1' => (false, '2'),
                _ => (true, '='),
            };
            if !carry {
                break;
            }
        }
    }

    fn decrement(digits: &mut [char]) {
        let mut carry;
        for digit in digits {
            (carry, *digit) = match *digit {
                '=' => (true, '2'),
                '-' => (false, '='),
                '0' => (false, '-'),
                '1' => (false, '0'),
                _ => (false, '1'),
            };
            if !carry {
                break;
            }
        }
    }

    loop {
        while n < num {
            snafu[pos] = match snafu[pos] {
                '=' => '-',
                '-' => '0',
                '0' => '1',
                '1' => '2',
                _ => {
                    increment(&mut snafu[pos + 1..]);
                    '='
                }
            };
            n += 5i64.pow(pos as u32);
        }
        if n == num {
            break;
        }
        n -= 5i64.pow(pos as u32);
        snafu[pos] = match snafu[pos] {
            '=' => {
                decrement(&mut snafu[pos + 1..]);
                '2'
            }
            '-' => '=',
            '0' => '-',
            '1' => '0',
            _ => '1',
        };
        pos -= 1;
    }
    snafu.iter().rev().skip_while(|&&c| c == '0').collect()
}

fn main() {
    let time = Instant::now();

    println!(
        "SNAFU number: {}",
        to_snafu(INPUT.lines().map(from_snafu).sum::<i64>())
    );

    println!(
        "elapsed: {} ms",
        time.elapsed().as_micros() as f32 / 1_000.0
    );
}
