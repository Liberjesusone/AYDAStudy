use std::io::{self, BufRead};
use std::fs::File;
use std::io::Write;

fn longest_increasing_subsequence(arr: &[i32]) -> i32 {
    0
}

fn main() {
    let stdin = io::stdin();
    let mut lines = stdin.lock().lines();

    let n: usize = lines.next().unwrap().unwrap().trim().parse().unwrap();
    let mut arr: Vec<i32> = Vec::with_capacity(n);

    for _ in 0..n {
        let item: i32 = lines.next().unwrap().unwrap().trim().parse().unwrap();
        arr.push(item);
    }

    let result = longest_increasing_subsequence(&arr);

    let mut file = File::create("solution.txt").expect("The file could not be created");
    writeln!(file, "{}", result).expect("The file could not be written to");
}