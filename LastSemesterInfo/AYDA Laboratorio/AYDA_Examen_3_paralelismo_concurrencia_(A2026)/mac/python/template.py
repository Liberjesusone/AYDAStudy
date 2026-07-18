"""
@file template.py
@brief Examen Práctico: Algoritmos Avanzados en Grafos y Concurrencia.

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
"""

import sys
import math
import threading
from typing import List, Dict, Optional, Tuple, Set
from graph import UndirectedGraph, DirectedGraph, Node, Arc, GraphException
from graph_algorithms import Kruskal, BellmanFord

# ========================================================================
# FUNCIONES AUXILIARES (NO MODIFICAR)
# ========================================================================

def find_node(graph, val: str) -> Optional[Node]:
    """
    Encuentra y devuelve el puntero a un nodo dado su valor string.
    """
    for node in graph.nodes:
        if node.value == val:
            return node
    return None

class BankAccount:
    """
    Estructura que representa una cuenta bancaria con control de concurrencia.
    """
    def __init__(self, account_id: int, balance: int):
        self.id = account_id
        self.balance = balance
        self.lock = threading.Lock()

# ========================================================================
# EJERCICIO 1: El Plan de Contingencia (Second-best MST)
# ========================================================================

def get_second_best_mst_weight(graph: UndirectedGraph) -> float:
    """
    Encuentra el peso (costo total) del Segundo Mejor Árbol de Recubrimiento Mínimo.

    @param graph: El grafo original no dirigido y ponderado.
    @return: float El peso total del segundo mejor MST.

    REGLA ESTRICTA: Si el grafo es estrictamente un árbol (o no tiene aristas de reemplazo
    que mantengan el grafo conectado), debes retornar -1.0.

    HINT 1: Usa Kruskal o Prim para encontrar el MST primario.
    HINT 2: Itera sobre cada arista del MST primario. Crea un subgrafo temporal excluyendo
            esa arista, calcula el MST de ese subgrafo y quédate con el de menor peso total.
    HINT 3: Para el cálculo del MST en cada iteración, puedes reutilizar el mismo algoritmo.
    """
    # TODO: Implementar el algoritmo para encontrar el Second-best MST.
    raise NotImplementedError("NOT_IMPLEMENTED")

# ========================================================================
# EJERCICIO 2: Núcleo Transaccional "GraphBank"
# ========================================================================

def safe_transfer(accounts: Dict[int, BankAccount], from_id: int, to_id: int, amount: int):
    """
    Motor de Transferencias Concurrentes Seguras.

    @param accounts Mapa de IDs de cuenta a punteros de BankAccount.
    @param from_id ID de la cuenta origen.
    @param to_id ID de la cuenta destino.
    @param amount Cantidad a transferir.

    CRÍTICO: Múltiples hilos llamarán a esta función al mismo tiempo.

    HINT: Usa std::lock_guard. Para evitar Interbloqueos (Deadlocks) cuando ocurren
       transferencias cruzadas (A->B y B->A simultáneas), debes establecer un
       "Orden Global" al adquirir los locks. ¿Qué pasa si siempre bloqueas primero el ID menor?
       Utiliza std::lock o bloquea en orden creciente de id.
       Recuerda verificar que haya saldo suficiente antes de realizar la transferencia.
    """
    # TODO: Implementar una transferencia atómica y Thread-Safe.
    # Elimina este return cuando implementes tu lógica.
    return

def execute_netting(debt_graph: DirectedGraph) -> str:
    """
    Liquidación de Deudas (Netting)

    @param debt_graph: Grafo dirigido donde el peso es la deuda de Origen hacia Destino.
    @return: str Lista de transferencias mínimas en formato "Origen->Destino:Monto"

    HINT: Calcula el balance neto de cada nodo (suma de entradas - suma de salidas).
       Luego, empareja a los nodos con balance negativo (deudores) con los de balance positivo (acreedores).
       El resultado debe ser una cadena con todas las transferencias necesarias, donde cada transferencia
       tiene el formato "Origen->Destino:Monto" y las transferencias se separan con un espacio.
       No debe haber espacio al final de la cadena. Por ejemplo: "EmpA->EmpC:150 EmpB->EmpC:75".
    """
    # TODO: Implementar el algoritmo de compensación de deudas (Netting).
    raise NotImplementedError("NOT_IMPLEMENTED")

# ========================================================================
# EJERCICIO 3: Arbitrage
# ========================================================================

def detect_arbitrage(graph: DirectedGraph) -> List[Node]:
    """
    Detectar ciclos de arbitraje que generen ganancias al multiplicar las tasas
    de cambio a lo largo del ciclo.

    @param graph: Grafo dirigido (pesos = tasas de cambio).
    @return: Lista de nodos que forman el ciclo de arbitraje.
    """
    # TODO: Implementar detección de ciclos de arbitraje.
    raise NotImplementedError("NOT_IMPLEMENTED")

# ========================================================================
# I/O BOILERPLATE (NO MODIFICAR)
# ========================================================================
if __name__ == "__main__":
    if len(sys.argv) < 2: sys.exit(1)
    exercise = sys.argv[1]

    with open("solution.txt", "w") as sol:
        try:
            if exercise == "ex1":
                g = UndirectedGraph.load_from_file("test_graph.txt", str, float)
                sol.write(f"{get_second_best_mst_weight(g)}\n")

            elif exercise == "ex2":
                with open("params.txt", "r") as p:
                    params = p.read().split()
                if params[0] == "netting":
                    g = DirectedGraph.load_from_file("test_graph.txt", str, float)
                    sol.write(f"{execute_netting(g)}\n")
                elif params[0] == "stress":
                    n_threads, iters = int(params[1]), int(params[2])
                    accs = {1: BankAccount(1, 1000000), 2: BankAccount(2, 1000000)}

                    def worker(thread_idx):
                        for j in range(iters):
                            if thread_idx % 2 == 0: safe_transfer(accs, 1, 2, 2)
                            else: safe_transfer(accs, 2, 1, 1)

                    threads = [threading.Thread(target=worker, args=(i,)) for i in range(n_threads)]
                    for t in threads: t.start()
                    for t in threads: t.join()

                    half_threads = n_threads // 2
                    expected_1 = 1000000 - (half_threads * iters * 2) + (half_threads * iters * 1)
                    expected_2 = 1000000 + (half_threads * iters * 2) - (half_threads * iters * 1)

                    if accs[1].balance == expected_1 and accs[2].balance == expected_2:
                        sol.write("SUCCESS\n")
                    else:
                        sol.write("CONSISTENCY_ERROR\n")

            elif exercise == "ex3":
                g = DirectedGraph.load_from_file("test_graph.txt", str, float)
                cycle = detect_arbitrage(g)
                sol.write(" ".join([n.value for n in cycle]) + "\n" if cycle else "\n")

        except NotImplementedError:
            sol.write("NOT_IMPLEMENTED\n")
        except Exception:
            sol.write("ERROR\n")