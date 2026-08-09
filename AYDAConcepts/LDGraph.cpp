// #include <algorithm>
// #include <array>
// #include <bitset>
// #include <cassert>
// #include <climits>
// #include <cmath>
// #include <cstring>
// #include <deque>
// #include <functional>
// #include <iomanip>
// #include <iterator>
// #include <limits>
// #include <list>
// #include <map>
// #include <numeric>
// #include <optional>
// #include <set>
// #include <sstream>
// #include <stack>
// #include <string>
// #include <tuple>
// #include <utility>

#include "include/LDGraph.hpp"
#include "include/GraphViewer.hpp"
#include <iostream>
#include <random>
#include <string>
#define endl '\n'
using namespace std;

/**
 * Exercise:
 * A directed, weighted graph built from random data: a random amount of
 * nodes (6-10), each with a random value, connected by a random amount of
 * directed edges with random weights.
 *
 * Print the nodes, then the visit order of DFS and BFS from node 0, then
 * open it in the interactive viewer.
 *
 * Note: since the graph is directed, DFS/BFS starting from node 0 are only
 * guaranteed to print the nodes REACHABLE from node 0 - not necessarily
 * every node in the graph. That's expected, not a bug: see for_each below,
 * which is the one that always visits every node regardless of direction.
 */
int main()
{
    random_device rd;
    mt19937_64 rng(rd());

    // -- random nodes: 6 to 10 of them, each with a random value in [1, 99]
    uniform_int_distribution<ull> node_count_dist(10, 20);
    uniform_int_distribution<int> value_dist(1, 1000);

    ull n = node_count_dist(rng);
    vector<int> values;
    for (ull i = 0; i < n; ++i) values.push_back(value_dist(rng));

    // -- random directed, weighted connections
    uniform_int_distribution<ull> id_dist(0, n - 1);
    uniform_int_distribution<ll> weight_dist(-20, 20);
    uniform_int_distribution<ull> edge_count_dist(n * 2, n * 4);   // enough to look connected

    vector<Connexion> conns;
    ull edge_count = edge_count_dist(rng);
    for (ull i = 0; i < edge_count; ++i)
        conns.push_back({ id_dist(rng), id_dist(rng), weight_dist(rng) });

    LDGraph<int> my_graph(values, conns);

    cout << "Nodos generados (" << n << "):\n";
    my_graph.for_each([](const auto& node) {
        cout << "  [" << node.get_id() << "] valor = " << node.get_value() << endl;
    });
    cout << endl;

    auto lambda = [](const int& value) -> void { cout << value << " -> "; };
    cout << "DFS : "; my_graph.for_eachDFS(lambda); cout << endl;
    cout << "BFS : "; my_graph.for_eachBFS(lambda); cout << endl;

    gviz::show(my_graph, [](const int& value) { return to_string(value); }, true, true);

    return 0;
}
