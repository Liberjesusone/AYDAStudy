#include <algorithm>
#include <bits/stdc++.h>
#include <utility>
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


/** @brief
 * Time limit: 1.00 s
 * Memory limit: 512 MB
 * 
 * You are in a book shop which sells n different books. You know the price and number of 
 * pages of each book.
 * You have decided that the total price of your purchases will be at most x. What is the 
 * maximum number of pages you can buy? You can buy each book at most once.
 * 
 * # Input
 * The first input line contains two integers n and x: the number of books and the maximum 
 * total price.
 * The next line contains n integers h_1,h_2,...,h_n: the price of each book.
 * The last line contains n integers s_1,s_2,...,s_n: the number of pages of each book.
 *
 * # Output
 * Print one integer: the maximum number of pages.
 * 
 * # Constraints
 * 1 <= n <= 1000
 * 1 <= x <= 10^5
 * 1 <= h_i, s_i <= 1000
 * 
 * # Example
 * Input:
 * 4 10
 * 4 8 5 3
 * 5 12 8 1
 * 
 * Output:
 * 13
 * 
 * Explanation: You can buy books 1 and 3. Their price is 4+5=9 and the number of pages is 5+8=13.
 */
int main()
{
    io_boost;

    int n, money; cin >> n; cin >> money;

    // prices and pages
    vector<pair<int, int>> books(n, make_pair(0, 0));
    for (ull i = 0; i < n; ++i) cin >> books[i].first;
    for (ull i = 0; i < n; ++i) cin >> books[i].second;

    // dp[i][m] is the best solution for m=money using a books[]'s sub-set from start that has i books 
    vector<vector<int>> dp(n+1, vector(money + 1, 0));

    for (ull book = 1; book <= n; ++book) // for every book
    {
        // for every money first price, +1, +2, +3, ...., money
        // the best next solution is wheter the previuos solution, the highest pages with nearts money
        // or the previous amount of books but for the rest of the money to include now the current book     
        for (ll m = 1; m <= money; ++m) 
        {
            auto price = books[book - 1].first;
            auto pages = books[book - 1].second;
            if (m >= price) 
                dp[book][m] = max(dp[book - 1][m], dp[book - 1][m - price] + pages);
            else // if we cannot buy the current book with inherit the previous solution
                dp[book][m] = dp[book - 1][m];
        }
    }

    cout << dp[n][money] << endl;

    return 0;
}