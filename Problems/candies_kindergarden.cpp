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


long candies(int n, vector<int> arr) 
{
    vector<int> values(n, 1); 

    for (int i = 0; i < n-1; ++i)
    {
        if (arr[i+1] > arr[i] && values[i+1] <= values[i]) 
            values[i+1] = values[i] + 1;
    } 
    for (int i = n-1; i > 0; --i)
    {
        if (arr[i-1] > arr[i] && values[i-1] <= values[i]) 
            values[i-1] = values[i] + 1;
    }
    
    long sum = 0;
    for (auto val : values) sum += val;

    return sum;
}


int main()
{
    io_boost;

    long n;    cin >> n;    vector<int> arr(n); 
    
    for (long i = 0; i < n; ++i) cin >> arr[i]; 
    
    long result = candies(n, arr);

    cout << result << "\n";

    return 0;
}