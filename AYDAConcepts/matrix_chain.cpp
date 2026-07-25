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
const ll oo = numeric_limits<ull>::max(); 


/** @brief Once the dp table is filled, we need to reconstruct the optimal parenthesization.
 * dp[i][j].second stores the optimal split point k for every sub-chain [i,j], so we just
 * have to read those k values and place the parentheses in the right places.
 *
 * We keep a vector of strings called result[], one per matrix, each initialized as "iX*"
 * where the trailing "*" acts as the multiplication operator to the right. The key invariant
 * is that every string in result[] always ends with "*", so inserting ")" at position end()-1
 * always places it just before that "*", no matter how many times the string has been modified.
 *
 * We use an explicit stack instead of recursion to avoid the call stack exploding for large n.
 * For each (i, j) we pop from the stack:
 *   - We open a "(" at result[i]  and close ")" at result[j] to wrap the whole group.
 *   - We close the left sub-group at result[k-1] and open the right sub-group at result[k],
 *     using the convention left = [i, k-1] and right = [k, j].
 *   - Then we push both sub-chains (i, k-1) and (k, j) for further expansion.
 *
 * Chains of length <= 1 are base cases (skip), since a single matrix needs no parentheses,
 * and a pair of two matrices already has their parens placed by the parent call.
 *
 * @returns a formatted string with the matrix header and the optimal parenthesization
 */
string str_solve(const vector<pair<ull, ull>>& _mtx, const vector<vector<pair<ull, ull>>>& dp) noexcept
{
    // First, we fill all the matriz with their indx in a header 
    string header = "";
    vector<string> result;    // vector that has the names of the matrix and will be filled with "(" ")"
    char letter = 'A';
    for (ull i = 0; i < _mtx.size(); ++i, ++letter) 
    {
        if (letter == 'Z') letter = 'A';

        header += to_string(i) + letter + "_{" + to_string(_mtx[i].first) + "x" + to_string(_mtx[i].second) + "}    "; 
        result.push_back(to_string(i) + letter + "*");

        if ((i+1) % 10 == 0) header += endl;
    }

    // Then we fill all the matrixs sub-chains
    stack<pair<ull, ull>> stack;
    stack.push(make_pair(0, _mtx.size()-1));
    for (ull i = 0, j = _mtx.size()-1; !stack.empty(); )
    {   
        auto pair = stack.top(); stack.pop();
        i = pair.first;    j = pair.second;

        if (j-i <= 1) continue;
        auto middle_k = dp[i][j].second;

        // We insert the parenthesis in the needed positions
        result[i] = "(" + result[i];
        result[j].insert(result[j].end() - 1, ')');
        result[middle_k-1].insert(result[middle_k-1].end() - 1, ')');
        result[middle_k] = "(" + result[middle_k];

        // We push the left and right sub-chains in the stack to continue the process
        stack.push(make_pair(i, middle_k-1));
        stack.push(make_pair(middle_k, j));
    }

    // Finally we join the result vect with the header 
    string res = "Matrixs:\n" + header + endl + endl + "Order:\n";
    for (ull i = 0; i < result.size(); ++i) res += result[i];

    res.pop_back(); // remove the last * character
    return res;
}



/** @brief Given a chain of n matrices, find the minimum number of scalar multiplications
 * needed to compute their product, and the optimal parenthesization that achieves it.
 *
 * The key observation is that multiplying a p×q matrix by a q×r matrix costs p*q*r scalar
 * multiplications. Matrix multiplication is associative, so (A*B)*C and A*(B*C) give the same
 * result but can cost completely different amounts. For example:
 *   A(10x30), B(30x5), C(5x60):
 *   (A*B)*C = 10*30*5 + 10*5*60 =  4500 operations
 *    A*(B*C) = 30*5*60 + 10*30*60 = 27000 operations
 * The number of possible parenthesizations grows exponentially (Catalan numbers), so brute
 * force is out. Greedy (e.g. always split at the smallest intermediate dimension) does not
 * guarantee the optimum either.
 *
 * This is where Dynamic Programming comes in. We define dp[i][j] as the minimum cost to
 * multiply all matrices from index i to index j. The recurrence is:
 *
 *   dp[i][j] = min over all k in [i+1, j] of:
 *       dp[i][k-1] + dp[k][j] + _mtx[i].first * _mtx[k].first * _mtx[j].second
 *
 * The logic is: for every sub-chain [i,j], there must be a "last multiplication" at some
 * split point k that divides the chain into left group [i, k-1] and right group [k, j].
 * The cost of that last multiplication is rows_of_left * shared_dim * cols_of_right, which
 * translates to _mtx[i].first * _mtx[k].first * _mtx[j].second (since compatible matrices
 * guarantee _mtx[k-1].second == _mtx[k].first). We add the cost of optimally solving both
 * sub-groups (already in the table) and keep the minimum over all k. Unlike greedy, we try
 * ALL possible split points, so we are guaranteed to find the optimum.
 *
 * We fill the table bottom-up by increasing chain length (separation), starting with pairs
 * of 2 matrices (separation=1) and growing to the full chain (separation=n-1). This ensures
 * that when we compute dp[i][j], all shorter sub-chains are already in the table, making
 * each lookup an O(1) operation.
 *
 * ## COMPLEXITY ANALYSIS
 *
 * Let n = number of matrices.
 *
 * Time: O(n^3)
 *   There are n*(n+1)/2 = O(n^2) unique sub-chains (i,j).
 *   For each sub-chain of length L we try L-1 = O(n) split points.
 *   Total: O(n^2) * O(n) = O(n^3).
 *
 * Space: O(n^2)
 *   The dp table is n x n pairs. The result string is O(n) in length.
 *   Total dominated by the dp table: O(n^2).
 *
 * @param _mtx vector of pairs (rows, cols) representing each matrix in order
 * @returns a pair: { min scalar multiplications, optimal parenthesization string }
 */
pair<ull, string> solve(const vector<pair<ull, ull>>& _mtx) noexcept
{
    if (_mtx.size() >= oo) return make_pair(0, "--");  // oo is not a usable value for this example

    // The dinamic matrix, for every dp[i][j] where i is the left matrix and j is the rigth one 
    // we will have pair, where first is the min amount of mult operations and second is the k position
    vector<vector<pair<ull, ull>>> dp(_mtx.size(), 
        vector<pair<ull, ull>>(_mtx.size(), make_pair(0, oo)));

    // returns the min and the k position value, comparing the two operations ways (A*B)*C >= A*(B*C)
    auto min_of_3 = [&dp, _mtx](ull i, ull j) -> pair<ull, ull> 
    {
        if (j-i != 2) throw invalid_argument("Not 3 separation index");

        // Matrix i_pxq            Matrix k_qxf         Matrix j_fxr   
        ull p = _mtx[i].first,    q = _mtx[i].second,     f = _mtx[j].first,      r = _mtx[j].second;
        ull PxQxF_PxFxR = p*q*f + p*f*r; // p*q*f + p*f*r 
        
        ull PxQxR_QxFxR = p*q*r + q*f*r; // p*q*r + q*f*r 

        pair<ull, ull> pair;
        if ( PxQxF_PxFxR >= PxQxR_QxFxR) // A*(B*C) 
        {                   
            pair.first = PxQxR_QxFxR;    // Prioritize the smallest 
            pair.second = i+1;           // k = i+1
            return pair;
        }
        else                             // (A*B)*C 
        {
            pair.first = PxQxF_PxFxR;
            pair.second = j;             // k = j
            return pair;
        }
    };

    // Calculates the min cost of operations for the sub-chain of matrix within this indexs
    auto cost_of = [&dp, &min_of_3, _mtx](ull i, ull j) -> pair<ull, ull> 
    {
        if (j-i == 1)      // two matrixs     min =  A_pxq*B_qxr  =      p*q*r
            return make_pair(_mtx[i].first*_mtx[i].second*_mtx[j].second, j);
        else if (j-i == 2) // three matrixs
            return min_of_3(i, j);
        
        // if the sub-chain contains more than 3 matrix we have to try all the n combinations
        // we try to split the sub-chain in the position k, that represents how many matrixs do
        // we have in the left group, and we keep the min of all the combinations
        ull _min = oo, min_k = 0;
        for (ull k = i+1; k <= j; ++k)
        {
            // the min cost of the left sub-chain + right sub-chain + p*q*r of the resulting 2 matrix multiplication
            // if we looks at a single matrix, dp[i][i].first would be 0, and is the spected output
            ull cost = dp[i][k-1].first + dp[k][j].first + 
                       _mtx[i].first*_mtx[k].first*_mtx[j].second;

            if (cost < _min)
            {
                _min = cost; 
                min_k = k; 
            }
        }

        return make_pair(_min, min_k);
    };
    
    // ull separation: represents a space for two matrix
    for (ull separation = 1; separation < _mtx.size(); ++separation)
    {
        for (ull i = 0; i + separation < _mtx.size(); ++i)
        {
            ull j = i + separation;
            dp[i][j] = cost_of(i, j);
        }
    }

    return make_pair(dp[0][_mtx.size()-1].first, str_solve(_mtx, dp));
}


/** @brief Matrix Chain Multiplication — reads n matrices and finds the optimal multiplication order.
 *
 * Given n matrices A_0, A_1, ..., A_{n-1} where each has dimensions rows x cols, the problem
 * is to find the parenthesization that minimizes the total number of scalar multiplications.
 * The result of multiplying all matrices is the same regardless of order (associativity),
 * but the cost can differ by orders of magnitude depending on how we group them.
 *
 * Input format:
 *   n              (number of matrices)
 *   rows cols      (dimensions of matrix 0)
 *   rows cols      (dimensions of matrix 1)
 *   ...
 *
 * Output: minimum number of operations, then the list of matrices and the optimal order.
 */
int main()
{
    io_boost;

    ull n; cin >> n; // amount of matrixs
            //rows / colums
    vector<pair<ull, ull>> matrixs; 
    for (ull i = 0; i < n; ++i) // we insert the n matrixs with their 2 parametters 
    {
        ull a, b; cin >> a; cin >> b;
        matrixs.push_back(make_pair(a, b));
    }

    auto res = solve(matrixs);

    cout << "Min operations: " << res.first << endl << endl << res.second << endl;

    return 0;
}