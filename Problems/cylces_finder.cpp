#include <bits/stdc++.h>
#include <vector>
using namespace std;
#define endl "\n"
//#include <bits/chronometer.h>
//using namespace chronometer;
#define io_boost ios_base::sync_with_stdio(0);cin.tie(0);
using ll = long long;
using ull = unsigned long long;
const ll MOD = 1e9 + 7;
const ll MAXN = 2e5 + 7;
const ull oo = numeric_limits<ull>::max(); // 18 - 446 744 073 - 709 551 615 = 1.8*10^{18}


/** @brief Finds all the simple cycles of a directed graph (it can have islands or sink nodes)
 *
 * Idea: every cycle has a unique "owner", its smallest node, so for every start node s we run
 * an iterative DFS that only walks through nodes >= s, and we only accept the cycles that close
 * back to s. That way a cycle can only be found from its owner, and only once from it:
 *
 * - the path is the line we are building, pos[node] tells in O(1) if node is in the path and where
 * - the stack keeps pairs {node, parent}, so when we pop a node whose parent is not the end of the
 *   path, we cut the path until the parent (the branch was exhausted, we are going back)
 * - at first I thought that we could achive it without visiting twice the nodes, but there is 
 *   NO global visited, a node can be walked many times through different paths, because
 *   each different path can close a different cycle
 * - a neighbor that is in the path but is not s also closes a cycle, but its owner is not s
 *   (its min is > s), so we ignore it, it will be found when that owner is the start
 * 
 * BigO : the time complextity of this algorithm would be O(V(V+E)) cause we are making a DFS 
 * search on every node, in order to garanty that we find all the cycles that starts at every node,
 * but here we are doing more that V+E on every DFS, cause we are not taking into account the visited
 * nodes, so now we cannot base the time complexity in V+E terms, so a better aproximation would be
 * O(V*SC*G) where V are the nodes, SC are the simple cycles that the graph contains and G the grade or
 * size of those cycles, in the worse escenario, this complexity takes into account 
 */
vector<vector<ull>> find_cycles(const vector<vector<ull>>& _graph) noexcept
{
    ull n = _graph.size();
    vector<vector<ull>> cycles;
    vector<ull> path;
    vector<ull> pos(n, oo); // pos[node] = index of node in the path, oo if it's not in the path

    for (ull s = 0; s < n; ++s)
    {
        stack<pair<ull, ull>> st; // {node, parent}
        st.push({s, oo});

        while (!st.empty())
        {
            auto [node, parent] = st.top(); st.pop();

            // 1. cut the path until the parent, so the path is again a straight line from s to node
            ull keep = (parent == oo) ? 0 : pos[parent] + 1;
            while (path.size() > keep)
            {
                pos[path.back()] = oo;
                path.pop_back();
            }
            pos[node] = path.size();
            path.push_back(node);

            // 2. look at the neighbors
            for (ull next : _graph[node])
            {
                if (next < s) continue; // cycles with a smaller node were already found by that node

                if (next == s) // closes a cycle owned by s
                {
                    if (path.size() < 2) continue; // self loop, a valid cycle needs >= 2 nodes
                    vector<ull> cycle = path;
                    cycle.push_back(s);
                    cycles.push_back(cycle);
                }
                else if (pos[next] == oo) st.push({next, node}); // not in the path, keep walking
            }
        }

        // 3. clean the path for the next start
        for (ull node : path) pos[node] = oo;
        path.clear();
    }

    return cycles;
}

/** @brief Recieves a graph that represents a net with directed conexions, our goal is to
 * determine whether or not it has cycles, a valid cycle is a path of lengh >= 2 nodes, that
 * gets to same starting point, just as a circle path.
 *
 * @returns "Contiene ciclo / No contiene ciclo" whether it has or not cycles respectively,
 * and also returns the paths by printing in the terminal the nodes as "[node_number]", and
 * representing the edges with -> so for the following input, the result output would be:
 *
 * @example:
 * # input: (the edges, any non digit char is ignored so it can be pasted as it is)
 *
 * {{0, 1}, {1, 2}, {2, 1}, {2, 3}, {3, 4}}
 *
 * # output:
 *
 * Contiene ciclo
 * [1] -> [2] -> [1]
 */
int main()
{
    io_boost;

    // 1. read the input line, all its numbers, every 2 numbers are an edge {from, to}
    string line;
    getline(cin, line);

    vector<ull> nums;
    string token;
    for (char c : line)
    {
        if (isdigit(c)) token += c;
        else if (!token.empty())
        {
            nums.push_back(stoull(token));
            token = "";
        }
    }
    if (!token.empty()) nums.push_back(stoull(token));

    ull n = 0;
    for (ull x : nums) n = max(n, x + 1);

    vector<vector<ull>> graph(n);
    for (ull i = 0; i + 1 < nums.size(); i += 2) graph[nums[i]].push_back(nums[i + 1]);
    for (auto& adj : graph) // remove repeated edges, so the same cycle is not found twice
    {
        sort(adj.begin(), adj.end());
        adj.erase(unique(adj.begin(), adj.end()), adj.end());
    }

    // 2. find and print the cycles
    vector<vector<ull>> cycles = find_cycles(graph);

    if (cycles.empty())
    {
        cout << "No contiene ciclo" << endl;
        return 0;
    }

    cout << "Contiene ciclo" << endl;
    for (auto& cycle : cycles)
    {
        for (ull i = 0; i < cycle.size(); ++i)
        {
            cout << "[" << cycle[i] << "]";
            if (i + 1 < cycle.size()) cout << " -> ";
        }
        cout << endl;
    }

    return 0;
}
