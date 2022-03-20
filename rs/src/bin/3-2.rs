fn most_common_bits(lines: &Vec<&str>, nbits: usize) -> Vec<i32> {
    let mut bits = vec![0; nbits];
    lines.iter().map(|line| line.chars()).for_each(|cs| {
        cs.enumerate().for_each(|(i, c)| match c {
            '1' => bits[i] += 1,
            '0' => bits[i] -= 1,
            _ => {}
        });
    });
    bits.iter().map(|&c| (c >= 0) as i32).collect::<Vec<i32>>()
}

fn bits_to_number(bits: &Vec<i32>) -> i32 {
    let n = bits.len();
    bits.iter()
        .enumerate()
        .fold(0, |acc, (i, b)| acc + 2_i32.pow((n - 1 - i) as u32) * b)
}

fn find_bit_pattern(lines: std::str::Lines, most: bool, nbits: usize) -> Vec<i32> {
    let mut remaining = lines.collect::<Vec<&str>>();

    for i in 0..nbits {
        let most_common = most_common_bits(&remaining, nbits)
            .iter()
            .nth(i)
            .map(|&b| {
                if most && b >= 1 || !most && b <= 0 {
                    '1'
                } else {
                    '0'
                }
            })
            .expect("There should be a count for every bit.");

        remaining = remaining
            .iter()
            .filter(|line| {
                line.chars()
                    .nth(i)
                    .map(|c| c == most_common)
                    .unwrap_or(false)
            })
            .map(|&line| line)
            .collect();

        if remaining.len() <= 1 {
            break;
        }
    }

    remaining.first()
        .expect("There should be a single value remaining.")
        .chars()
        .map(|c| match c {
            '1' => 1,
            _ => 0,
        })
        .collect::<Vec<i32>>()
}

fn life_support(input: &str) -> i32 {
    let nbits = input
        .lines()
        .next()
        .expect("Input is empty when it should contain multiple lines.")
        .len();

    let oxy = bits_to_number(&find_bit_pattern(input.lines(), true, nbits));
    let co2 = bits_to_number(&find_bit_pattern(input.lines(), false, nbits));

    oxy * co2
}

fn main() -> std::io::Result<()> {
    let input = std::fs::read_to_string("../3.txt")?;
    println!("Power consumption: {}", life_support(&input));
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test_power_consumption() {
        let input =
            "00100\n11110\n10110\n10111\n10101\n01111\n00111\n11100\n10000\n11001\n00010\n01010";
        assert_eq!(life_support(input), 230);
    }
}
