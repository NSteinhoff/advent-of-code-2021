fn calc_imp(input: &str) -> i32 {
    let mut depth = 0;
    let mut horizontal = 0;

    for line in input.lines() {
        let parts = line.split_once(" ");
        if let Some((command, magnitude)) = parts {
            if let Ok(magnitude) = magnitude.parse::<i32>() {
                match command {
                    "forward" => horizontal += magnitude,
                    "up" => depth -= magnitude,
                    "down" => depth += magnitude,
                    _ => {}
                }
            }
        }
    }

    depth * horizontal
}

fn calc_fun(input: &str) -> i32 {
    let (depth, horizontal) = input
        .lines()
        .filter_map(|line| line.split_once(" "))
        .filter_map(|(c, m)| m.parse::<i32>().map(|m| (c, m)).ok())
        .fold((0, 0), |(d, h), (c, m)| match c {
            "forward" => (d, h + m),
            "up" => (d - m, h),
            "down" => (d + m, h),
            _ => (d, h),
        });

    depth * horizontal
}

fn main() -> std::io::Result<()> {
    let input = std::fs::read_to_string("../2.txt")?;

    println!("Imperative: {}", calc_imp(&input));
    println!("Functional: {}", calc_fun(&input));

    Ok(())
}

#[cfg(test)]
mod tests {
    #[test]
    fn calculate_test_input() {
        use super::*;
        let input = "forward 5\ndown 5\nforward 8\nup 3\ndown 8\nforward 2";
        assert_eq!(calc_imp(input), 150);
        assert_eq!(calc_imp(input), calc_fun(input));
    }
}
