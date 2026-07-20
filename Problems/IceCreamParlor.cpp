#include <bits/stdc++.h>
using namespace std;
#define endl "\n"
//#include <bits/chronometer.h>
//using namespace chronometer;
#define io_boost ios_base::sync_with_stdio(0);cin.tie(0);
using ll = long long;
using ull = unsigned long long;
const ll MOD = 1e9 + 7;
const ll MAXN = 2e5 + 7;
const ll oo = numeric_limits<ll>::max(); // 2*10^10

int main()
{
    io_boost;

    ull money, n; cin >> money; cin >> n;
    vector<ull> prices(n);
    for (ull i = 0; i < n; ++i) cin >> prices[i];

    // the price and indx
    unordered_map<ull, ull> visited_prices;

    for (ull i = 0; i < n; ++i)
    {
        ull diff = money - prices[i];
        if (!visited_prices.contains(diff)) 
        {
            visited_prices[prices[i]] = i;
            continue;
        }
        cout << visited_prices[diff]+1 << " " << i+1 << endl;
    }

    return 0;
}

/**
example
1000
10
10
50
60
40
300
608
50
700
900
2
*/