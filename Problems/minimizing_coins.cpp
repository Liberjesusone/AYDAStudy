#include <bits/stdc++.h>
#include <string>
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


ull solve(vector<ull>& _coins, ull money) noexcept
{
    // If money is oo (INF) is our limit and also a reserved mark 
    if (money >= oo || money == 0) return oo; 
    
    // -- First of all calculate the dinamic programing table
    vector<ull> dp(money+1, oo);
    sort(_coins.begin(), _coins.end()); // We sort the array 
    if (_coins[0] >= money+1) return oo;
    dp[0] = 0;    dp[_coins[0]] = 1;                // the smallest price completable

    for (ull m = _coins[0] + 1; m <= money; ++m)    // For every completable amount of money 
    {   
        for (ull i = 0; i < _coins.size() && m >= _coins[i]; ++i)     // For every coin we have
            if (dp[m - _coins[i]] != oo)
                dp[m] = min(dp[m - _coins[i]] + 1, dp[m]);
    }

    return dp[money] == oo ? oo : dp[money];
}

/** @brief 
 * Time limit: 1.00 s
 * Memory limit: 512 MB
 * 
 * Consider a money system consisting of n coins. Each coin has a positive integer value. 
 * Your task is to produce a sum of money x using the available coins in such a way that 
 * the number of coins is minimal.
 * For example, if the coins are {1,5,7} and the desired sum is 11, an optimal solution 
 * is 5+5+1 which requires 3 coins.
 * 
 * # Input
 * The first input line has two integers n and x: the number of coins and the desired sum of money.
 * The second line has n distinct integers c_1,c_2,...,c_n: the value of each coin.
 * 
 * # Output
 * Print one integer: the minimum number of coins. If it is not possible to produce the desired sum, print -1.
 *
 * # Constraints
 * 1 <= n <= 100
 * 1 <= x <= 10^6
 * 1 <= c_i <= 10^6
 * 
 * # Example
 * Input:
 * 3 11
 * 1 5 7
 * 
 * Output:
 * 3
 */
int main()
{
    io_boost;

    ull money, n; cin >> n; cin >> money;
    vector<ull> coins(n);

    for (ull i = 0; i < n; ++i) cin >> coins[i];
    
    ull res = solve(coins, money);
    cout << (res == oo ? to_string(-1) : to_string(res)) << endl; 

    return 0;
}