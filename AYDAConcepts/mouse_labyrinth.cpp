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

// marks the path to follow to find the E char with X, only finds a path, not the best one 
void solve(vector<vector<char>> &_maze, ull n, pair<ll, ll> s_location)
{
    using coord = pair<ll, ll>;

    // is the node _maze[i][j] visited -> visited[i][j]
    vector<vector<bool>> visited(n, vector<bool>(n, false)); 
    
    // whenever we jump to explore another node, we save the parent we are jumping from
    vector<vector<coord>> parent(n, vector<coord>(n, {-1, -1})); 

    // 1. first node to search
    stack<coord> stack; 
    stack.push(coord(s_location));
    bool found = false;
    coord end_loc;

    // 2. while there are still nodes to search 
    while(!stack.empty())
    {
        auto next = stack.top(); stack.pop(); 

        // 3. if the node has not been visited, is a wall or out of bounds
        if (next.first >= n || next.first < 0 || 
            next.second >= n || next.second < 0) continue;

        if (visited[next.first][next.second] || _maze[next.first][next.second] == '#') continue;

        // 4. we search on that node 
        visited[next.first][next.second] = true;
        if (_maze[next.first][next.second] == 'E') 
        {
            end_loc = coord(next.first, next.second);
            found = true; 
            break;
        }

        // 5. if we didn't found it, we search on his childs by stacking them
        vector<coord> neighbors = {{1,0}, {-1,0}, {0,1}, {0,-1}};
        for (int i = 0; i < 4; ++i)
        {
            coord child = {next.first + neighbors[i].first, next.second + neighbors[i].second}; 
            if (child.first >= 0 && child.first < n && child.second >= 0 && child.second < n)
            {
                if (!visited[child.first][child.second] && _maze[child.first][child.second] != '#'
                    && parent[child.first][child.second] == coord{-1, -1})
                {
                    parent[child.first][child.second] = next;
                    stack.push(coord(child.first, child.second));
                }
            }
        }
    }
    
    if (found)
    {
        auto current = end_loc;
        while (current != s_location && current.first != -1)
        {
            _maze[current.first][current.second] = 'X'; // we mark that we have pass through this node
            current = parent[current.first][current.second];
        }
    }

    return;
}


int main()
{
    io_boost;

    ll n; cin >> n; 
    vector<vector<char>> maze(n, vector<char>(n , '.')); 
    for (ll i = 0; i < n; ++i) 
        for (ll j = 0; j < n; ++j)
            cin >> maze[i][j];
    pair<ll, ll> s_loc;
    cin >> s_loc.first;     cin >> s_loc.second; 

    solve(maze, n, s_loc);

    for (auto vec : maze)
    {
        for (auto c : vec)
            cout << c; 
        cout << endl;
    }

    return 0;
}

/*
20
S...................
###################.
....................
.###################
...................#
##################.#
...................#
.###################
...................#
##################.#
...................#
.###################
...................#
##################.#
...................#
.###################
...................#
##################.#
...................#
...................E
0 0

*/