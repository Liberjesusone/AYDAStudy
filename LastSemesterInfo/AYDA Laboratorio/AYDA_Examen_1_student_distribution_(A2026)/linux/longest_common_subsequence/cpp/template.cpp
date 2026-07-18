#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

vector<int> longestCommonSubsequence(vector<int> a, vector<int> b)
{
    return vector<int>();
}

int main()
{
    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int> a(n), b(m);
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < m; ++i) cin >> b[i];

    vector<int> result = longestCommonSubsequence(a, b);

    ofstream fout("solution.txt");
    for (size_t i = 0; i < result.size(); ++i) {
        fout << result[i] << (i == result.size() - 1 ? "" : " ");
    }
    fout << "\n";
    fout.close();
    return 0;
}