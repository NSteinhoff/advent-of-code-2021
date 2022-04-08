fn final_score(input: &str) -> i32 {
    let mut lines = input.lines();

    let numbers: Vec<i32> = lines
        .next()
        .expect("Input was empty")
        .split(",")
        .filter_map(|s| s.parse::<i32>().ok())
        .collect();

    let rows: Vec<Vec<usize>> = (0..5)
        .map(|row| (0..5).map(move |col| row * 5 + col).collect::<Vec<usize>>())
        .collect();
    let cols: Vec<Vec<usize>> = (0..5)
        .map(|col| (0..5).map(move |row| col + row * 5).collect::<Vec<usize>>())
        .collect();

    let mut boards: Vec<Vec<i32>> = vec![];
    for line in lines {
        if line == "" {
            boards.push(vec![]);
        } else {
            line.split(" ")
                .filter_map(|n| n.parse::<i32>().ok())
                .for_each(|n| {
                    boards
                        .last_mut()
                        .expect("Did you forget to create a board?")
                        .push(n)
                });
        }
    }

    let mut picks = boards
        .iter()
        .map(|_| vec![false; 25])
        .collect::<Vec<Vec<bool>>>();

    let mut winners: Vec<(usize, i32)> = vec![];
    for number in numbers {
        for (i, board) in boards.iter().enumerate() {
            // Stop marking tiles on winning boards
            if winners.iter().find(|(k, _n)| *k == i).is_some() {
                continue;
            }

            for (j, _n) in board.iter().enumerate().filter(|(_i, &n)| n == number) {
                picks[i][j] = true;

                let is_winner = rows
                    .iter()
                    .find(|row| {
                        row.iter()
                            .fold(0, |picked, j| picked + (picks[i][*j] as i32))
                            == 5
                    })
                    .is_some()
                    || cols
                        .iter()
                        .find(|col| {
                            col.iter()
                                .fold(0, |picked, j| picked + (picks[i][*j] as i32))
                                == 5
                        })
                        .is_some();

                if is_winner {
                    winners.push((i, number));
                    break;
                }
            }
        }
    }

    println!("Winners: {:?}", winners);

    let (i, number) = *winners.first().expect("Someone has to win!");
    let sum_unmarked =
        boards[i]
            .iter()
            .enumerate()
            .fold(0, |sum, (j, n)| if picks[i][j] { sum } else { sum + n });

    println!("Sum unmarked: {}", sum_unmarked);

    sum_unmarked * number
}

fn main() -> std::io::Result<()> {
    let input = std::fs::read_to_string("../4.txt")?;
    println!("\nFinal Score: {}", final_score(&input));
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const INPUT: &str = "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7";

    #[test]
    fn test_final_score() {
        assert_eq!(final_score(INPUT), 4512);
    }
}
