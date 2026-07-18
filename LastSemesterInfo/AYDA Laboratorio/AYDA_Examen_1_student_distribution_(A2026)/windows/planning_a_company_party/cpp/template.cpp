#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

struct Employee
{
    double rating;
    int left_child;
    int right_sibling;
};

double planParty(const vector<Employee>& tree)
{
    return 0.0;
}

int main()
{
    int n;
    if (!(cin >> n)) return 0;

    vector<Employee> tree(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> tree[i].rating >> tree[i].left_child >> tree[i].right_sibling;
    }

    double result = planParty(tree);

    ofstream fout("solution.txt");
    fout << fixed << setprecision(2) << result << "\n";
    fout.close();
    return 0;
}