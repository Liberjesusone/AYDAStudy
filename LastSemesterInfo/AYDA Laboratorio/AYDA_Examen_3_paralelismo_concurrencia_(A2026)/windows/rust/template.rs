/// @file template.rs
/// @brief Examen Práctico: Algoritmos Avanzados en Grafos y Concurrencia.

// Declaramos los archivos adyacentes como módulos
mod graph;
mod graph_algorithms;

use crate::graph::{DirectedGraph, UndirectedGraph, Node, Graph};
use crate::graph_algorithms::{Kruskal, BellmanFord};
use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use std::thread;
use std::fs::File;
use std::io::{Write, Read};
use std::rc::Rc;

/*
INSTRUCCIONES:
- Completa las funciones marcadas con "TODO".
- NO modifiques las firmas de las funciones existentes.
- NO modifiques el bloque "main" ni la función "find_node".
- Puedes agregar funciones auxiliares (helpers) si lo consideras necesario.

REGLAS IMPORTANTES:
- EJERCICIO 1: Si el grafo es estrictamente un árbol (o no tiene aristas de reemplazo
  que mantengan el grafo conectado), debes retornar -1.0.
- EJERCICIO 2 (Segundo Punto): Para evitar deadlocks, adquiere los mutexes en orden global
  (por ID de cuenta) y usa el contexto de threading (Lock).
*/

// ========================================================================
// FUNCIONES AUXILIARES (NO MODIFICAR)
// ========================================================================

/// Encuentra y devuelve el puntero a un nodo dado su valor.
fn find_node<N, W, G>(graph: &G, val: N) -> Option<Rc<Node<N>>>
where
    N: PartialEq + Clone,
    G: Graph<N, W>,
{
    for node in graph.get_nodes() {
        if node.value == val {
            return Some(node.clone());
        }
    }
    None
}

/// Estructura que representa una cuenta bancaria con control de concurrencia.
pub struct BankAccount {
    pub id: usize,
    pub balance: i32,
}

// ========================================================================
// EJERCICIO 1: El Plan de Contingencia (Second-best MST)
// ========================================================================

/// Encuentra el peso (costo total) del Segundo Mejor Árbol de Recubrimiento Mínimo.
///
/// @param graph: El grafo original no dirigido y ponderado.
/// @return: f64 El peso total del segundo mejor MST.
///
/// REGLA ESTRICTA: Si el grafo es estrictamente un árbol (o no tiene aristas de reemplazo
/// que mantengan el grafo conectado), debes retornar -1.0.
///
/// HINT 1: Usa Kruskal o Prim para encontrar el MST primario.
/// HINT 2: Itera sobre cada arista del MST primario. Crea un subgrafo temporal excluyendo
///         esa arista, calcula el MST de ese subgrafo y quédate con el de menor peso total.
/// HINT 3: Para el cálculo del MST en cada iteración, puedes reutilizar el mismo algoritmo.
pub fn get_second_best_mst_weight<N, W>(graph: &UndirectedGraph<N, W>) -> f64
where
    N: Clone + std::cmp::Eq + std::hash::Hash + 'static,
    W: Clone + Copy + PartialOrd + Default + std::ops::Add<Output = W> + Into<f64> + 'static,
{
    // TODO: Implementar el algoritmo para encontrar el Second-best MST.
    unimplemented!("NOT_IMPLEMENTED")
}

// ========================================================================
// EJERCICIO 2: Núcleo Transaccional "GraphBank"
// ========================================================================

/// Motor de Transferencias Concurrentes Seguras.
///
/// @param accounts Mapa de IDs de cuenta a punteros de BankAccount.
/// @param from_id ID de la cuenta origen.
/// @param to_id ID de la cuenta destino.
/// @param amount Cantidad a transferir.
///
/// CRÍTICO: Múltiples hilos llamarán a esta función al mismo tiempo.
///
/// HINT: Usa MutexGuard. Para evitar Interbloqueos (Deadlocks) cuando ocurren
///       transferencias cruzadas (A->B y B->A simultáneas), debes establecer un
///       "Orden Global" al adquirir los locks. ¿Qué pasa si siempre bloqueas primero el ID menor?
///       Utiliza un bloqueo en orden creciente de id.
///       Recuerda verificar que haya saldo suficiente antes de realizar la transferencia.
pub fn safe_transfer(accounts: &HashMap<usize, Arc<Mutex<BankAccount>>>, from_id: usize, to_id: usize, amount: i32) {
    // TODO: Implementar una transferencia atómica y Thread-Safe.
}

/// Liquidación de Deudas (Netting)
///
/// @param debt_graph: Grafo dirigido donde el peso es la deuda de Origen hacia Destino.
/// @return: String Lista de transferencias mínimas en formato "Origen->Destino:Monto"
///
/// HINT: Calcula el balance neto de cada nodo (suma de entradas - suma de salidas).
///       Luego, empareja a los nodos con balance negativo (deudores) con los de balance positivo (acreedores).
///       El resultado debe ser una cadena con todas las transferencias necesarias, donde cada transferencia
///       tiene el formato "Origen->Destino:Monto" y las transferencias se separan con un espacio.
///       No debe haber espacio al final de la cadena. Por ejemplo: "EmpA->EmpC:150 EmpB->EmpC:75".
pub fn execute_netting<N, W>(debt_graph: &DirectedGraph<N, W>) -> String
where
    N: std::fmt::Display + Clone + std::cmp::Eq + std::hash::Hash,
    W: Clone + Copy + std::ops::Sub<Output = W> + std::ops::Add<Output = W> + Default + PartialOrd + std::fmt::Display + 'static,
{
    // TODO: Implementar el algoritmo de compensación de deudas (Netting).
    unimplemented!("NOT_IMPLEMENTED")
}

// ========================================================================
// EJERCICIO 3: Arbitrage
// ========================================================================

/// Detectar ciclos de arbitraje que generen ganancias al multiplicar las tasas
/// de cambio a lo largo del ciclo.
///
/// @param graph: Grafo dirigido (pesos = tasas de cambio).
/// @return: Lista de nodos que forman el ciclo de arbitraje.
pub fn detect_arbitrage<N, W>(graph: &DirectedGraph<N, W>) -> Vec<Rc<Node<N>>>
where
    N: Clone + std::cmp::Eq + std::hash::Hash,
    W: Clone + Copy + Default + PartialOrd + std::ops::Add<Output = W> + 'static,
{
    // TODO: Implementar detección de ciclos de arbitraje.
    unimplemented!("NOT_IMPLEMENTED")
}

// ========================================================================
// I/O BOILERPLATE (NO MODIFICAR)
// ========================================================================

fn main() -> std::io::Result<()> {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 { std::process::exit(1); }
    let exercise = &args[1];

    let mut sol = File::create("solution.txt")?;

    let result = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| -> std::io::Result<String> {
        let mut output = String::new();
        match exercise.as_str() {
            "ex1" => {
                let g = UndirectedGraph::<String, f64>::load_from_file("test_graph.txt")?;
                output = format!("{}\n", get_second_best_mst_weight(&g));
            }
            "ex2" => {
                let mut p_file = File::open("params.txt")?;
                let mut p_content = String::new();
                p_file.read_to_string(&mut p_content)?;
                let params: Vec<&str> = p_content.split_whitespace().collect();

                if params[0] == "netting" {
                    let g = DirectedGraph::<String, f64>::load_from_file("test_graph.txt")?;
                    output = format!("{}\n", execute_netting(&g));
                } else if params[0] == "stress" {
                    let n_threads: usize = params[1].parse().unwrap();
                    let iters: usize = params[2].parse().unwrap();

                    // FIX: stdArc -> Arc
                    let accs = HashMap::from([
                        (1, Arc::new(Mutex::new(BankAccount { id: 1, balance: 1_000_000 }))),
                        (2, Arc::new(Mutex::new(BankAccount { id: 2, balance: 1_000_000 }))),
                    ]);

                    let mut threads = vec![];
                    for i in 0..n_threads {
                        let accs_clone = accs.clone();
                        threads.push(thread::spawn(move || {
                            for _ in 0..iters {
                                if i % 2 == 0 { safe_transfer(&accs_clone, 1, 2, 2); }
                                else { safe_transfer(&accs_clone, 2, 1, 1); }
                            }
                        }));
                    }
                    for t in threads { t.join().unwrap(); }

                    let b1 = accs[&1].lock().unwrap().balance;
                    let b2 = accs[&2].lock().unwrap().balance;

                    let half_threads = (n_threads / 2) as i32;
                    let iters_i32 = iters as i32;
                    let expected_1 = 1_000_000 - (half_threads * iters_i32 * 2) + (half_threads * iters_i32 * 1);
                    let expected_2 = 1_000_000 + (half_threads * iters_i32 * 2) - (half_threads * iters_i32 * 1);

                    if b1 == expected_1 && b2 == expected_2 {
                        output = "SUCCESS\n".to_string();
                    } else {
                        output = "CONSISTENCY_ERROR\n".to_string();
                    }
                }
            }
            "ex3" => {
                let g = DirectedGraph::<String, f64>::load_from_file("test_graph.txt")?;
                let cycle = detect_arbitrage(&g);
                let cycle_str = cycle.iter().map(|n| n.value.clone()).collect::<Vec<_>>().join(" ");
                output = format!("{}\n", cycle_str);
            }
            _ => {}
        }
        Ok(output)
    }));

    match result {
        Ok(Ok(out)) => { write!(sol, "{}", out)?; }
        Ok(Err(_)) => { writeln!(sol, "ERROR")?; }
        Err(_) => { writeln!(sol, "NOT_IMPLEMENTED")?; }
    }

    Ok(())
}