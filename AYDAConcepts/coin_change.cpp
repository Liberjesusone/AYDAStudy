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
const ull oo = numeric_limits<ull>::max(); // 18 - 446 744 073 - 709 551 615 = 1.8*10^{18}


/** @brief if we find the the minimun amount of coins for the initial cases of a dinamic programing table
 * called dp[m], where "m" is the money we want to complete, so if we have the table completed till k like 
 * { dp[1], dp[2], dp[3], ..., dp[k-1], dp[k] }  where k = m - 1, we can calculate then k+1 = m, just by
 * iterating all the coins in the following logic, if the coins array is c[n]
 * dp[m-c[0]] + 1 = a_0
 * dp[m-c[1]] + 1 = a_1
 * dp[m-c[2]] + 1 = a_2
 * dp[m-c[3]] + 1 = a_3
 *     .
 *     .
 *     .
 * dp[m-c[n]] + 1 = a_n
 *  
 * We are basically calculating the amount of coins that are needed to complete m, by testing it 
 * with all the coins we have, e.g imaging that we try to complete 6 (m) using the coin with value 2
 * (c[n]), that is one of the coins in the array, so we will need the smallest amount of coins to 
 * complete 6-2 = 4 (dp[m-c[n]]), we look in the dp table which is bassically an O(1) operation 
 * when is already calculated dp[m-c[n]], and we note down this number and sum 1 to take into account 
 * the coin with value 2 that we are using apart from the dp[m-c[n]], that's the number a_0, then we 
 * do the same with all the others coins, because the solution may be on the others coins, and we 
 * will find all the others a_n numbers, that are all the amount of coins that we need to use to complete
 * m, tested on all the coins, and the min of those a_n with be our answer.
 * 
 * So in order to find a dp[m+1] we repeat this process, so the question now is, how do we calculate the 
 * first cases, we will need to use the smallest coin the set of coins. and then work with it.
 * 
 * If we don't have the coin one, we are not guaranteed to complete any amount of money, so we have a mark.
 * If a dp[n] = oo (INF) that means that we cannot complete it using the given coins. In that case we will return 0
 * @returns 0 if cannot the money cannot be completed using the givin coins, and the min. amount of coins otherwise
 */
ull solve(vector<ull>& _coins, ull money) noexcept
{
    // If money is oo (INF) is our limit and also a reserved mark 
    if (money >= oo || money == 0) return 0; 
    
    // -- First of all calculate the dinamic programing table
    vector<ull> dp(money+1, oo);
    sort(_coins.begin(), _coins.end()); // We sort the array 
    dp[0] = 0;    dp[_coins[0]] = 1;                // the smallest price completable

    for (ull m = _coins[0] + 1; m <= money; ++m)    // For every completable amount of money 
    {   
        for (ull i = 0; i < _coins.size() && m >= _coins[i]; ++i)     // For every coin we have
            if (dp[m - _coins[i]] != oo)
                dp[m] = min(dp[m - _coins[i]] + 1, dp[m]);
    }

    return dp[money] == oo ? 0 : dp[money];
}

/** @brief We use the same logic as before but we create the dp[] table with the
 * maximun price of the list of prices that is sent as a parameter @param _max_price
 * after this we create the result vector just by doing O(1) consults on the dp[] table
 * n times, where n is the size of @param _prices and each consult is bassically the 
 * result minimun amount of coins needed to complete _price[n] using the given coins
 * 
 * @returns a vector of pairs where the first element of every pair is the price that
 * we wanted to complete, and the second is the min amount of coins for that
 */
vector<pair<ull, ull>> solve(vector<ull>& _coins, vector<ull>& _prices, ull _max_price) noexcept
{
    vector<pair<ull, ull>> vec; 

    // If money is oo (INF) is our limit and also a reserved mark 
    if (_max_price >= oo || _max_price == 0) return vec; 
    
    // -- First of all calculate the dinamic programing table
    vector<ull> dp(_max_price+1, oo);
    sort(_coins.begin(), _coins.end()); // We sort the array 
    dp[0] = 0;    dp[_coins[0]] = 1;                // the smallest price completable

    for (ull m = _coins[0] + 1; m <= _max_price; ++m)    // For every completable amount of money 
    {   
        for (ull i = 0; i < _coins.size() && m >= _coins[i]; ++i)     // For every coin we have
            if (dp[m - _coins[i]] != oo)
                dp[m] = min(dp[m - _coins[i]] + 1, dp[m]);
    }

    // -- Then we fill all the consults in the resulting vector
    for (ull i = 0; i < _prices.size(); ++i)
        vec.push_back(make_pair(_prices[i], dp[_prices[i]] == oo ? 0 : dp[_prices[i]]));

    return vec;
}

int main_with_only_one_case()
{
    ull money, n; cin >> money; cin >> n;
    vector<ull> coins(n);

    for (ull i = 0; i < n; ++i) cin >> coins[i];
    
    ull res = solve(coins, money);
    cout << res << endl;
    
    return 0;
}


int main_with_more_moneys()
{
    ull t, n_prices, n; cin >> t; cin >> n_prices; cin >> n;
    vector<ull> coins(n);
    vector<ull> prices(n_prices);
    
    for (ull i = 0; i < t; ++i) // For every test case we receive a list of prices and coins 
    {
        ull max_price = 0;
        for (ull i = 0; i < n; ++i) cin >> coins[i];
        for (ull i = 0; i < n_prices; ++i) 
        {
            cin >> prices[i];
            max_price = max(max_price, prices[i]);
        }
        
        cout << "TEST CASE " << i << endl;
        vector<pair<ull, ull>> res = solve(coins, prices, max_price);
        for (ull j = 0; j < res.size(); ++j) 
            cout << "Price: " << res[j].first << "  -Count: " << res[j].second << endl;
    }

return 0;
}

int main()
{
    io_boost;

    
    int res; cin >> res;
    if (res == 0) main_with_only_one_case(); 
    else main_with_more_moneys();

    return 0;
}

/*
Single tests with 0 at the beggining: 0

TC1 — greedy falla, DP gana:
0
6 3
1 3 4
Esperado: 2 (3+3, no 4+1+1)

TC2 — cantidad imposible:
0
7 2
3 5
Esperado: 0 (7 no se puede formar con {3,5})

TC3 — una sola moneda, múltiplo exacto:
0
10 1
5
Esperado: 2

TC4 — una sola moneda, no múltiplo:
0
7 1
5
Esperado: 0

TC5 — caso clásico de concurso:
0
41 4
1 5 10 25
Esperado: 4 (25+10+5+1)

TC6 — monedas repetidas en el input:
0
6 3
2 2 2
Esperado: 3

TC7 — money = 0:
0
0 3
1 2 5
Esperado: 0


TC basic — 2 test cases, 3 prices, 3 coins:
1
2 3 3
1 3 4
3 6 7
1 2 5
4 7 11

Spected output:
TEST CASE 0
Price: 3  -Count: 1
Price: 6  -Count: 2
Price: 7  -Count: 2
TEST CASE 1
Price: 4  -Count: 2
Price: 7  -Count: 2
Price: 11  -Count: 3

TC with impossible prices:
1
1 4 2
3 5
1 3 7 8

Spected output:
TEST CASE 0
Price: 1  -Count: 0
Price: 3  -Count: 1
Price: 7  -Count: 0
Price: 8  -Count: 2
 */