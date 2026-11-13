#include <bits/stdc++.h>
#include <fstream> // the local bits/stdc++.h of the repo doesn't have it (HackerRank does)

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'bfs' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER m
 *  3. 2D_INTEGER_ARRAY edges
 *  4. INTEGER s
 */

/** @brief BFS: Shortest Reach, finds the shortest distance from _s to every other node of an
 * undirected graph where every edge weighs 6
 *
 * - as all the edges weigh the same, the shortest path is the one with the least amount of edges,
 *   and BFS visits the nodes exactly in that order: first the ones at 1 edge, then the ones at 2...
 * - so the first time that a node is discovered its distance is already the shortest one, and
 *   dist[v] = dist[u] + 6, where u is the node that discovered v
 * - dist[v] = -1 means "not discovered yet", and if it stays like that, v is unreachable
 *
 * BigO: O(n + m), every node enters the queue once and every edge is checked twice (undirected)
 *
 * @returns the distances to the nodes 1..n in increasing order, without the start node
 */
vector<int> bfs(int n, int m, vector<vector<int>> edges, int s) {
    const int WEIGHT = 6;

    // 1. adjacency list, the nodes are 1..n so we use n + 1 spaces and ignore the 0
    vector<vector<int>> graph(n + 1);
    for (auto& edge : edges) {
        int u = edge[0], v = edge[1];
        graph[u].push_back(v); // undirected, so the edge goes in both directions
        graph[v].push_back(u);
    }

    // 2. BFS from s, the dist also works as the visited (-1 = not visited)
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);

    while (!q.empty()) {
        int u = q.front(); q.pop();

        for (int v : graph[u]) {
            if (dist[v] != -1) continue; // already discovered with a shorter or equal distance

            dist[v] = dist[u] + WEIGHT;
            q.push(v);
        }
    }

    // 3. the answer is every node in order, except the start node
    vector<int> result;
    for (int node = 1; node <= n; ++node) {
        if (node == s) continue;
        result.push_back(dist[node]);
    }

    return result;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string q_temp;
    getline(cin, q_temp);

    int q = stoi(ltrim(rtrim(q_temp)));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string first_multiple_input_temp;
        getline(cin, first_multiple_input_temp);

        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        int n = stoi(first_multiple_input[0]);

        int m = stoi(first_multiple_input[1]);

        vector<vector<int>> edges(m);

        for (int i = 0; i < m; i++) {
            edges[i].resize(2);

            string edges_row_temp_temp;
            getline(cin, edges_row_temp_temp);

            vector<string> edges_row_temp = split(rtrim(edges_row_temp_temp));

            for (int j = 0; j < 2; j++) {
                int edges_row_item = stoi(edges_row_temp[j]);

                edges[i][j] = edges_row_item;
            }
        }

        string s_temp;
        getline(cin, s_temp);

        int s = stoi(ltrim(rtrim(s_temp)));

        vector<int> result = bfs(n, m, edges, s);

        for (size_t i = 0; i < result.size(); i++) {
            fout << result[i];

            if (i != result.size() - 1) {
                fout << " ";
            }
        }

        fout << "\n";
    }

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
