import sys

from graph import UndirectedGraph, DirectedGraph, Node
from graph_algorithms import GraphTraversals, GraphProperties, GraphTopological

# ========================================================================
# HELPER FUNCTION (Provided for you)
# ========================================================================
def find_node(graph, val: str) -> Node:
    """Finds a node in the graph by its string value."""
    for node in graph.nodes:
        if node.value == val:
            return node
    return None

# ========================================================================
# EXERCISE 1: The Main Event (Wrestling Rivalries)
# ========================================================================
def classify_wrestlers(roster: UndirectedGraph) -> tuple[bool, set, set]:
    """
    TODO: Implement your logic here.
    Return a tuple: (is_possible: bool, faces: set, heels: set)
    """
    faces = set()
    heels = set()

    return False, faces, heels

# ========================================================================
# EXERCISE 2: The Phantom Heist (Strict Waypoints)
# ========================================================================
def plan_heist(mansion: UndirectedGraph, entry: Node, vault: Node, escape: Node) -> list[Node]:
    """
    TODO: Implement your logic here.
    Return a list of nodes representing the shortest path: Entry -> Vault -> Escape.
    If impossible, return an empty list [].
    """
    return []

# ========================================================================
# EXERCISE 3: Network Triage (Prioritization)
# ========================================================================
def get_repair_priority(network: UndirectedGraph) -> list[list[Node]]:
    """
    TODO: Implement your logic here.
    Return a list of lists (isolated network segments), sorted by size (descending).
    """
    return []

# ========================================================================
# EXERCISE 4: Parallel Build Orchestrator
# ========================================================================
def get_parallel_build_schedule(project_modules: DirectedGraph) -> list[list[Node]]:
    """
    TODO: Implement your logic here.
    Return a list of lists (stages). Each stage must be sorted alphabetically by node.value.
    If the graph has a cycle, return an empty list.
    """
    return []

# ========================================================================
# I/O BOILERPLATE (DO NOT MODIFY BELOW THIS LINE)
# This section communicates with the Automated Grader.
# ========================================================================
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Error: No exercise specified.")
        sys.exit(1)

    exercise = sys.argv[1]

    with open("solution.txt", "w") as sol:
        if exercise == "ex1":
            roster = UndirectedGraph.load_from_file("test_graph.txt", str, float)
            possible, faces, heels = classify_wrestlers(roster)

            sol.write("1\n" if possible else "0\n")
            if possible:
                sol.write(" ".join([n.value for n in faces]) + "\n")
                sol.write(" ".join([n.value for n in heels]) + "\n")

        elif exercise == "ex2":
            mansion = UndirectedGraph.load_from_file("test_graph.txt", str, float)

            with open("params.txt", "r") as params:
                lines = params.read().splitlines()
                e_val, v_val, esc_val = lines[0], lines[1], lines[2]

            entry = find_node(mansion, e_val)
            vault = find_node(mansion, v_val)
            escape = find_node(mansion, esc_val)

            route = plan_heist(mansion, entry, vault, escape)
            sol.write(" ".join([n.value for n in route]) + "\n")

        elif exercise == "ex3":
            network = UndirectedGraph.load_from_file("test_graph.txt", str, float)
            segments = get_repair_priority(network)

            for seg in segments:
                sol.write(" ".join([n.value for n in seg]) + "\n")

        elif exercise == "ex4":
            dag = DirectedGraph.load_from_file("test_graph.txt", str, float)
            stages = get_parallel_build_schedule(dag)

            if not stages:
                sol.write("CYCLE\n")
            else:
                for stage in stages:
                    sol.write(" ".join([n.value for n in stage]) + "\n")