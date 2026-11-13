#include <bits/stdc++.h>
using namespace std;
#define endl "\n"
#include <bits/chronometer.h>
using namespace chronometer;
#define io_boost ios_base::sync_with_stdio(0);cin.tie(0);
using ll = long long;
using ull = unsigned long long;
const ll MOD = 1e9 + 7;
const ll MAXN = 2e5 + 7;
const ull oo = numeric_limits<ull>::max(); // 18 - 446 744 073 - 709 551 615 = 1.8*10^{18}


/**
 * # Statement 1
 * Objetivo: Encontrar el camino minimo desde el nodo origen 0 (almacen central) hasta el nodo
 * destino 5 (Punto z)
 *
 * Topologia oficial: (grafo dirigido ponderado)
 * - Conjunto de Nodos (V) : {0,1,2,3,4,5}
 * - Lista de aristas con sus pesos estrictamente positivos (u -> v: w):
 *     - 0 -> 1 (peso 4)
 *     - 0 -> 2 (peso 2)
 *     - 1 -> 2 (peso 1)
 *     - 1 -> 3 (peso 5)
 *     - 2 -> 3 (peso 8)
 *     - 2 -> 4 (peso 10)
 *     - 3 -> 4 (peso 2)
 *     - 3 -> 5 (peso 6)
 *     - 4 -> 5 (peso 3)
 *
 * # Statement 2
 * Objetivo: Calcular los caminos mínimos desde el nodo de origen 0 (Gateway-Principal) y verificar 
 * si la red es segura frente a anomalías financieras.
 *
 * Topologia Oficial (Grafo dirigido con pesos mixtos y ciclo negativo)
 * - Conjunto de nodos (V) : {0,1,2,3,4}
 * - Lista de arista con sus pesos (u -> v: w)
 *    - 0 -> 1 (peso 4)
 *    - 0 -> 2 (peso 5)
 *    - 1 -> 3 (peso 3)
 *    - 2 -> 1 (peso -2) (arista con peso negativo)
 *    - 3 -> 2 (peso -6) (Crea un cilo de peso negativo: 1->3->2->1 con costo total -5)
 *    - 2 -> 4 (peso 2)
 *    - 3 -> 4 (peso 1)
 * 
 * # Statement 3
 * Objetivo: encnotrar la ruta optima desde el nodo origen A hasta el nodo destino D en un espacio fisico
 * bidimensional
 * 
 * Topologia oficial (grafo con coordenadas catersianas (x,y) y distancias euclidianas)
 * - Coordenadas de los nodos:
 *    - A (0,0) [Origen]
 *    - B (1,4)
 *    - C (4,1)
 *    - D (5,5) [Destino]
 * - Conexinoes y distancias reales (costo de las aristas)
 *    - A -> B (costo real 4.5)
 *    - A -> C (costo real 4.2)
 *    - B -> D (costo real 4.1)
 *    - B -> C (costo real 3.5)
 *   - C -> D (costo real 4.3)
 */

// adjacency list: graph[u] = list of {v, w} meaning a directed edge u -> v with weight w
// W is the type of the weights, ll for the statements 1 and 2, double for the statement 3 (decimals)
template <typename W>
using Graph = vector<vector<pair<ll, W>>>;

// distance of a node that has not been reached yet
template <typename W>
const W inf = numeric_limits<W>::max();

/** @brief Dijkstra, finds the shortest distance from _src to every node of a directed weighted graph
 *
 * How it works:
 * - dist[v] is the best known distance from _src to v, at the start it's inf for everyone but _src = 0
 * - a min priority queue keeps the pairs {dist, node}, so we always pop the node with the smallest
 *   known distance that has not been finished yet
 * - the greedy key: when a node u is popped with the smallest distance, that distance is FINAL, because
 *   any other path to u has to go out through a node with a bigger or equal distance, and adding more
 *   edges can only make it bigger (that's why it only works when all the weights are >= 0, a negative
 *   edge could make a "longer" path cheaper later, and the greedy decision would be wrong)
 * - then we "relax" every edge u -> v: if dist[u] + w < dist[v] we found a better path to v, so we update
 *   dist[v], save u as the parent of v (to rebuild the path later) and push v again in the queue
 * - a node can be in the queue many times with old distances, so when we pop a pair whose distance is
 *   bigger than dist[node] it's outdated and we skip it (lazy deletion, cheaper than a decrease-key)
 *
 * BigO: O((V + E) log V), every edge can push once in the queue and every push/pop costs log
 *
 * @returns {dist, parent}, parent[v] = previous node of v in its shortest path, -1 if it has none,
 * or nullopt if it found a negative weight (the error is printed and dijkstra stops right there)
 */
template <typename W>
optional<pair<vector<W>, vector<ll>>> dijkstra(const Graph<W>& _graph, ll _src) noexcept
{
    ll n = _graph.size();
    vector<W> dist(n, inf<W>);
    vector<ll> parent(n, -1);

    // min heap of {dist, node}, greater<> makes the top the smallest one
    priority_queue<pair<W, ll>, vector<pair<W, ll>>, greater<pair<W, ll>>> pq;
    dist[_src] = 0;
    pq.push({0, _src});

    while (!pq.empty())
    {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue; // outdated pair, u was already finished with a better distance

        for (auto [v, w] : _graph[u])
        {
            if (w < 0) // dijkstra's greedy choice is not valid with negative weights
            {
                cout << "Error: no es posible desarrollar Dijkstra con pesos negativos (arista "
                     << u << " -> " << v << " con peso " << w << ")" << endl;
                return nullopt;
            }

            // relax the edge u -> v
            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    return make_pair(dist, parent);
}

/** @brief prints the path from _src to _dst as [u] -> (w) [v] -> ... and its total cost, if _names
 * is not empty the nodes are printed with their names instead of their numbers
 */
template <typename W>
void print_path(const Graph<W>& _graph, const vector<W>& _dist, const vector<ll>& _parent, ll _src, ll _dst,
                const vector<string>& _names) noexcept
{
    auto label = [&](ll _node) { return _names.empty() ? to_string(_node) : _names[_node]; };

    if (_dist[_dst] == inf<W>)
    {
        cout << "No existe camino desde [" << label(_src) << "] hasta [" << label(_dst) << "]" << endl;
        return;
    }

    // 1. rebuild the path going backwards through the parents, then reverse it
    vector<ll> path;
    for (ll node = _dst; node != -1; node = _parent[node]) path.push_back(node);
    reverse(path.begin(), path.end());

    // 2. print every node with the weight of the edge that leads to the next one
    cout << "[" << label(path[0]) << "]";
    for (ll i = 1; i < (ll)path.size(); ++i)
    {
        ll u = path[i - 1], v = path[i];
        W w = inf<W>; // weight of u -> v, the smallest one in case there are repeated edges
        for (auto [to, weight] : _graph[u]) if (to == v) w = min(w, weight);
        cout << " -> (" << w << ") [" << label(v) << "]";
    }
    cout << endl;
    cout << "Costo total = " << _dist[_dst] << endl;
}

/** @brief builds the official sample graph of the statement 1
 */
Graph<ll> sample_graph_1() noexcept
{
    ll n = 6;
    Graph<ll> graph(n);
    vector<tuple<ll, ll, ll>> edges = { // {u, v, w}
        {0, 1, 4}, {0, 2, 2}, {1, 2, 1}, {1, 3, 5}, {2, 3, 8},
        {2, 4, 10}, {3, 4, 2}, {3, 5, 6}, {4, 5, 3}
    };
    for (auto [u, v, w] : edges) graph[u].push_back({v, w});
    return graph;
}

/** @brief builds the official sample graph of the statement 2 (it has negative weights)
 */
Graph<ll> sample_graph_2() noexcept
{
    ll n = 5;
    Graph<ll> graph(n);
    vector<tuple<ll, ll, ll>> edges = { // {u, v, w}
        {0, 1, 4}, {0, 2, 5}, {1, 3, 3}, {2, 1, -2},
        {3, 2, -6}, {2, 4, 2}, {3, 4, 1}
    };
    for (auto [u, v, w] : edges) graph[u].push_back({v, w});
    return graph;
}

/** @brief builds the official sample graph of the statement 3, the weights are the real distances
 * between the points of the plane, so they are decimals (double)
 * A = 0 (0,0), B = 1 (1,4), C = 2 (4,1), D = 3 (5,5)
 */
Graph<double> sample_graph_3() noexcept
{
    ll n = 4;
    Graph<double> graph(n);
    vector<tuple<ll, ll, double>> edges = { // {u, v, w}
        {0, 1, 4.5}, {0, 2, 4.2}, {1, 3, 4.1}, {1, 2, 3.5}, {2, 3, 4.3}
    };
    for (auto [u, v, w] : edges) graph[u].push_back({v, w});
    return graph;
}

/** @brief prints the header of a statement, runs dijkstra from _src and prints the path to every
 * node in _dsts, and also the time that it took (even if it failed)
 */
template <typename W>
void run_statement(const string& _name, const Graph<W>& _graph, ll _src, const vector<ll>& _dsts,
                   const vector<string>& _names = {}) noexcept
{
    cout << "----------------------------" << endl;
    cout << "Dijkstra" << endl;
    cout << _name << endl;

    clock_start();
    auto result = dijkstra(_graph, _src);
    if (result) // if it failed the error was already printed, we only show how long it took to fail
    {
        auto& [dist, parent] = *result;
        for (ll dst : _dsts) print_path(_graph, dist, parent, _src, dst, _names);
    }
    long long us = clock_end_us(); // read the clock only once, so ms and us are the same measure
    cout << "Tiempo = " << us / 1000 << " ms (" << us << " us)" << endl;
    cout << endl;
}

/** @brief Dijkstra for the 3 statements
 *
 * # output: (the times change in every execution)
 * ----------------------------
 * Dijkstra
 * Statement 1
 * [0] -> (4) [1] -> (5) [3] -> (2) [4] -> (3) [5]
 * Costo total = 14
 * Tiempo = 0 ms (_ us)
 *
 * ----------------------------
 * Dijkstra
 * Statement 2
 * Error: no es posible desarrollar Dijkstra con pesos negativos (arista 2 -> 1 con peso -2)
 * Tiempo = 0 ms (_ us)
 *
 * ----------------------------
 * Dijkstra
 * Statement 3
 * [A] -> (4.2) [C] -> (4.3) [D]
 * Costo total = 8.5
 * Tiempo = 0 ms (_ us)
 */
int main()
{
    io_boost;

    // statement 1: from the central warehouse (0) to the point z (5)
    run_statement("Statement 1", sample_graph_1(), 0, {5});

    // statement 2: from the main gateway (0) to every node, it fails because of the negative weights
    run_statement("Statement 2", sample_graph_2(), 0, {1, 2, 3, 4});

    // statement 3: from A (0) to D (3) in the plane
    run_statement("Statement 3", sample_graph_3(), 0, {3}, {"A", "B", "C", "D"});

    return 0;
}

/* # Sample graph 1 (directed, weights between parentheses)
 *
 *              (4)            (5)
 *        [0] -------> [1] -------> [3] -----(6)-----+
 *         |            |          ^  |              |
 *         |           (1)        /   |              v
 *        (2)           |       (8)  (2)            [5]
 *         |            v       /     |              ^
 *         +---------> [2] ----+      v              |
 *                      |            [4] -----(3)----+
 *                      |             ^
 *                      +----(10)-----+
 *
 * Shortest path 0 -> 5:  [0] -(4)-> [1] -(5)-> [3] -(2)-> [4] -(3)-> [5]  = 14
 */

/* # Sample graph 2 (directed, weights between parentheses)
 *
 *              (4)          (3)
 *        [0] ------> [1] ------> [3]
 *         |           ^         /  |
 *         |         (-2)    (-6)  (1)
 *        (5)          |     /      |
 *         |           |    v       v
 *         +--------> [2] ---(2)--> [4]
 *
 * Negative cycle: [1] -(3)-> [3] -(-6)-> [2] -(-2)-> [1]  = -5
 * Dijkstra stops when it relaxes the edge 2 -> 1 with weight -2
 */

/* # Sample graph 3 (directed, points of the plane, real distances between parentheses)
 *
 *   y
 *   5 |                                  D (5,5)
 *   4 |    B (1,4) -------(4.1)-------->   ^
 *     |     ^   \                          |
 *   3 |     |    \                         |
 *   2 |   (4.5)  (3.5)                   (4.3)
 *     |     |      \                       |
 *   1 |     |       +------------> C (4,1) +
 *     |     |                       ^
 *   0 |  A (0,0) -------(4.2)-------+
 *     +-------------------------------------------- x
 *
 * Shortest path A -> D:  [A] -(4.2)-> [C] -(4.3)-> [D]  = 8.5   (the other one A -> B -> D costs 8.6)
 */
