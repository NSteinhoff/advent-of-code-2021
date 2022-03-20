fn power_consumption(input: &str) -> i32 {
    let nbits = input
        .lines()
        .next()
        .expect("Input is empty when it should contain multiple lines.")
        .len();

    let mut bits = vec![0; nbits];

    input.lines().map(|line| line.chars()).for_each(|cs| {
        cs.enumerate().for_each(|(i, c)| match c {
            '1' => bits[i] += 1,
            '0' => bits[i] -= 1,
            _ => {}
        });
    });


    let mut most_common = bits.iter().map(|&c| (c >= 0) as i32).collect::<Vec<i32>>();
    most_common.reverse();

    let gamma = most_common
        .iter()
        .enumerate()
        .fold(0, |acc, (i, b)| acc + 2_i32.pow(i as u32)*b);
    let epsilon = most_common
        .iter()
        .map(|&v| (v == 0) as i32)
        .enumerate()
        .fold(0, |acc, (i, b)| acc + 2_i32.pow(i as u32)*b);

    gamma * epsilon
}

fn main() -> std::io::Result<()> {
    let input = std::fs::read_to_string("../3.txt")?;
    println!("Power consumption: {}", power_consumption(&input));
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test_power_consumption() {
        let input =
            "00100\n11110\n10110\n10111\n10101\n01111\n00111\n11100\n10000\n11001\n00010\n01010";
        assert_eq!(power_consumption(input), 198);
    }
}
