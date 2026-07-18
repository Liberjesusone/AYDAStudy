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

#include "include/LGraph.hpp"
#include <iostream>
#define endl '\n'
using namespace std;

/**
 * Exercise:
 * 5 nodes, 5 edges (not directed):
 * 1-2, 1-3, 2-4, 3-5, 4-5
 *
 * Print the visit order of BFS and DFS from the node 1.
 */
int main()
{
    auto lambda = [](const ull val) -> void { cout << val << " -> "; };
    auto list = { make_pair(1ULL, 2ULL), make_pair(1ULL, 3ULL), make_pair(2ULL, 4ULL), make_pair(3ULL, 5ULL), make_pair(4ULL, 5ULL) };
    LGraph<ull> my_graph(list);

    cout << "DFS : "; my_graph.for_eachDFS(lambda); cout << endl;
    cout << "BFS : "; my_graph.for_eachBFS(lambda); cout << endl;

    return 0;
}