fn count_increases(input: String) -> i32 {
    let (_, count) = input
        .lines()
        .filter_map(|line| line.parse::<i32>().ok())
        .collect::<Vec<i32>>()
        .as_slice()
        .windows(3)
        .fold((None, 0), |(previous, count), window| {
            let current = window.iter().fold(0, |acc, n| acc + n);
            (
                Some(current),
                previous.map_or(count, |p| if current > p { count + 1 } else { count }),
            )
        });

    count
}

fn main() -> std::io::Result<()> {
    let input = std::fs::read_to_string("../1.txt")?;
    println!("Count: {}", count_increases(input));
    Ok(())
}
