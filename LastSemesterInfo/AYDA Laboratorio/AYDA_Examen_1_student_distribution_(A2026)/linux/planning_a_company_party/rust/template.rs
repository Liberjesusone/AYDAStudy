use std::io::{self, Read};
use std::fs::File;
use std::io::Write;

struct Employee {
    rating: f64,
    left_child: i32,
    right_sibling: i32,
}

fn plan_party(tree: &[Employee]) -> f64 {
    0.0
}

fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut tokens = input.split_whitespace();

    if let Some(n_str) = tokens.next() {
        let n: usize = n_str.parse().unwrap();
        let mut tree = Vec::with_capacity(n);

        for _ in 0..n {
            let rating: f64 = tokens.next().unwrap().parse().unwrap();
            let left_child: i32 = tokens.next().unwrap().parse().unwrap();
            let right_sibling: i32 = tokens.next().unwrap().parse().unwrap();
            tree.push(Employee { rating, left_child, right_sibling });
        }

        let result = plan_party(&tree);

        let mut file = File::create("solution.txt").unwrap();
        writeln!(file, "{:.2}", result).unwrap();
    }
}