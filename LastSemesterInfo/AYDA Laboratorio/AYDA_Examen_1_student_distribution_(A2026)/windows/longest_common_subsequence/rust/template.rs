use std::io::{self, Read};
use std::fs::File;
use std::io::Write;

fn longest_common_subsequence(a: &[i32], b: &[i32]) -> Vec<i32> {
    vec![]
}

fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut tokens = input.split_whitespace();

    if let (Some(n_str), Some(m_str)) = (tokens.next(), tokens.next()) {
        let n: usize = n_str.parse().unwrap();
        let m: usize = m_str.parse().unwrap();

        let mut a = Vec::with_capacity(n);
        for _ in 0..n { a.push(tokens.next().unwrap().parse().unwrap()); }

        let mut b = Vec::with_capacity(m);
        for _ in 0..m { b.push(tokens.next().unwrap().parse().unwrap()); }

        let result = longest_common_subsequence(&a, &b);

        let mut file = File::create("solution.txt").unwrap();
        let strings: Vec<String> = result.iter().map(|n| n.to_string()).collect();
        writeln!(file, "{}", strings.join(" ")).unwrap();
    }
}