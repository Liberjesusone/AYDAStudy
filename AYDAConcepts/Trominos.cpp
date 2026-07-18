#include <bits/stdc++.h>
using namespace std;
#define endl "\n"
//#include <bits/chronometer.h>
//using namespace chronometer;
using ll = long long;
using ull = unsigned long long;
static ull marked_cells = 0;


int main()
{
    ll n; cin >> n; // power of n
    ll cell; cin >> cell;
    if (n < 1) { cout << "No valid n : " << n << endl; return 0;}
    if (cell < 0) { cout << "No valid deleted cell : " << cell << endl; return 0;}
    ull _pow = pow(2, n);
    ull cells = _pow * _pow;

    vector<vector<ull>> board;


    return 0;
}