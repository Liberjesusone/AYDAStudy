#include <bits/stdc++.h>
#include <unordered_set>
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
 * Time limit: 1.00 s Memory limit: 512 MB
 * Your task is to place eight queens on a chessboard so that no two queens are 
 * attacking each other. As an additional challenge, each square is either free or reserved, 
 * and you can only place queens on the free squares. However, the reserved squares do not prevent 
 * queens from attacking each other.
 * 
 * How many possible ways are there to place the queens?
 * 
 * # Input
 * The input has eight lines, and each of them has eight characters. Each square is either free (.) 
 * or reserved (*).
 * 
 * # Output
 * Print one integer: the number of ways you can place the queens.
 * 
 * Example
 * Input:
 * ........
 * ........
 * ..*.....
 * ........
 * ........
 * .....**.
 * ...*....
 * ........
 * 
 * Output:
 * 65
 */
int main()
{
    io_boost;


    ull n = 8;  vector<string> board(n);
    for (ull i = 0; i < n; i++) cin >> board[i];

    ull count = 0;
    stack<ull> stack;                                     // stack that holds the iteration index for every recursion leve
    stack.push(0);
    unordered_set<ull> rows, cols, diag_right, diag_left; // holds the indx of the diagnal, rows and cols that are attacked
    unordered_map<ull, ull> queens;
    bool has_inserted = false;
    
    auto remove_queen = [&queens, &rows, &cols, &diag_right, &diag_left, &n](ull row) -> void 
    {
        ull col = queens[row];
        queens.erase(row);
        diag_left.erase(col + row);           cols.erase(col);
        diag_right.erase(n - 1 - col + row);  rows.erase(row);  

    };

    while(!stack.empty())
    {
        has_inserted = false;
        ull indx = stack.top(); stack.pop();
        ull row = rows.size(); // = queens_count -> if we have introduced n queens most be in n separated rows 

        // try to insert the current queen in a spot
        for (ull col = indx; col < board.size() && row < n; ++col) 
        {
            if (diag_left.contains(col + row) || cols.contains(col) ||          // is not an available spot, or it's 
                diag_right.contains(n - 1 - col + row) || rows.contains(row) || // an attacked row, col or diagnal
                board[row][col] == '*')                                              
                continue;

            // if not, we insert the queen and jump to another row
            diag_left.insert(col + row);           cols.insert(col);
            diag_right.insert(n - 1 - col + row);  rows.insert(row);  
            queens[row] = col;        

            // stack the next recursion call
            stack.push(col);    stack.push(0);
            has_inserted = true;
            break;
        }
        
        // we have to pop up the last queen because it was a wrong branch
        if (!has_inserted)              // if we have successfully inserted all queens
        {                               // then row == n
            if (stack.empty()) break;
            ++stack.top();              // we will continue with the next possible spot
            remove_queen(row - 1); // remove the last queen
            if (row == n) ++count;
        }
    }

    cout << count << endl;

    return 0;
}
