use std::fs;

fn main() -> std::io::Result<()> {
    println!("USAGE: cargo run --bin [problem]");
    println!("\nProblems:");
    let mut bins: Vec<String> = fs::read_dir("src/bin/")?
        .filter_map(|f| f.ok())
        .filter_map(|f| f.file_name().into_string().ok())
        .filter_map(|f| f.rsplit_once(".").map(|(name, _)| name.to_owned()))
        .filter(|f| f != "main")
        .map(|f| f.to_owned())
        .collect();

    bins.sort();
    bins.iter().for_each(|f| println!("\t{}", f));

    Ok(())
}
