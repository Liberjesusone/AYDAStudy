use std::env;
use std::fs::File;
use std::io::{self, BufRead, Write};
use std::collections::HashSet;
use std::rc::Rc;

use crate::graph::{DirectedGraph, UndirectedGraph, Graph, Node};
use crate::graph_algorithms::{GraphTraversals, GraphProperties, GraphTopological};

// ========================================================================
// HELPER FUNCTION (Provided for you)
// Finds a node in the graph by its string value.
// ========================================================================
fn find_node(graph: &impl Graph<String, f64>, val: &str) -> Option<Rc<Node<String>>> {
    graph.get_nodes().iter().find(|n| n.value == val).cloned()
}

// ========================================================================
// EXERCISE 1: The Main Event (Wrestling Rivalries)
// ========================================================================
pub fn classify_wrestlers(
    roster: &UndirectedGraph<String, f64>,
    faces: &mut HashSet<Rc<Node<String>>>,
    heels: &mut HashSet<Rc<Node<String>>>
) -> bool {
    // TODO: Implement your logic here using BFS or DFS.
    // Return true if possible, and populate 'faces' and 'heels'.

    false
}

// ========================================================================
// EXERCISE 2: The Phantom Heist (Strict Waypoints)
// ========================================================================
pub fn plan_heist(
    mansion: &UndirectedGraph<String, f64>,
    entry: Rc<Node<String>>,
    vault: Rc<Node<String>>,
    escape: Rc<Node<String>>
) -> Vec<Rc<Node<String>>> {
    // TODO: Implement your logic here.
    // Return the shortest path. If impossible, return an empty Vec.

    Vec::new()
}

// ========================================================================
// EXERCISE 3: Network Triage (Prioritization)
// ========================================================================
pub fn get_repair_priority(
    network: &UndirectedGraph<String, f64>
) -> Vec<Vec<Rc<Node<String>>>> {
    // TODO: Implement your logic here.
    // Return a list of isolated network segments, sorted by size (descending).

    Vec::new()
}

// ========================================================================
// EXERCISE 4: Parallel Build Orchestrator
// ========================================================================
pub fn get_parallel_build_schedule(
    project_modules: &DirectedGraph<String, f64>
) -> Vec<Vec<Rc<Node<String>>>> {
    // TODO: Implement your logic here.
    // Tip: Use `GraphProperties::has_cycle` first to avoid panicking!
    // Return stages sorted alphabetically by node.value. If a cycle exists, return empty Vec.

    Vec::new()
}

// ========================================================================
// I/O BOILERPLATE (DO NOT MODIFY BELOW THIS LINE)
// This section communicates with the Automated Grader.
// ========================================================================
fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        eprintln!("Error: No exercise specified.");
        std::process::exit(1);
    }

    let exercise = &args[1];
    let mut sol = File::create("solution.txt")?;

    match exercise.as_str() {
        "ex1" => {
            let roster = UndirectedGraph::<String, f64>::load_from_file("test_graph.txt")?;
            let mut faces = HashSet::new();
            let mut heels = HashSet::new();

            let possible = classify_wrestlers(&roster, &mut faces, &mut heels);

            if possible {
                writeln!(sol, "1")?;
                for n in faces { write!(sol, "{} ", n.value)?; }
                writeln!(sol)?;
                for n in heels { write!(sol, "{} ", n.value)?; }
                writeln!(sol)?;
            } else {
                writeln!(sol, "0")?;
            }
        },
        "ex2" => {
            let mansion = UndirectedGraph::<String, f64>::load_from_file("test_graph.txt")?;

            let params_file = File::open("params.txt")?;
            let mut lines = io::BufReader::new(params_file).lines();

            let e_val = lines.next().unwrap().unwrap();
            let v_val = lines.next().unwrap().unwrap();
            let esc_val = lines.next().unwrap().unwrap();

            let entry = find_node(&mansion, &e_val).unwrap();
            let vault = find_node(&mansion, &v_val).unwrap();
            let escape = find_node(&mansion, &esc_val).unwrap();

            let route = plan_heist(&mansion, entry, vault, escape);
            for n in route { write!(sol, "{} ", n.value)?; }
            writeln!(sol)?;
        },
        "ex3" => {
            let network = UndirectedGraph::<String, f64>::load_from_file("test_graph.txt")?;
            let segments = get_repair_priority(&network);

            for seg in segments {
                for n in seg { write!(sol, "{} ", n.value)?; }
                writeln!(sol)?;
            }
        },
        "ex4" => {
            let dag = DirectedGraph::<String, f64>::load_from_file("test_graph.txt")?;
            let stages = get_parallel_build_schedule(&dag);

            if stages.is_empty() {
                writeln!(sol, "CYCLE")?;
            } else {
                for stage in stages {
                    for n in stage { write!(sol, "{} ", n.value)?; }
                    writeln!(sol)?;
                }
            }
        },
        _ => eprintln!("Unknown exercise: {}", exercise),
    }

    Ok(())
}