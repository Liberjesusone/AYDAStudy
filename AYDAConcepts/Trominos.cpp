#include <bits/stdc++.h>
using namespace std;
#define endl "\n"
//#include <bits/chronometer.h>
//using namespace chronometer;
using ll = long long;
using ull = unsigned long long;

static ull CURRENT_TROMINOE = 1;
static ull DEL_ROW{};
static ull DEL_COL{};
static ull _SIZE{};

// ----- FUNCTIONS NOT NEEDED FOR THIS PROBLEM: --------------------
/** @brief returns the 2*2 quadrant of the cell, given the number of cells per column/row
 */
pair<ull, ull> quadrant_of(const ull& _row, const ull& _col) noexcept
{
    return make_pair((_row / 2) * 2, (_col / 2) * 2);
}

/** @brief returns the 2*2 quadrant of the cell, given the number of cells per column/row
 */
pair<ull, ull> quadrant_of_cell(const ull& _cell, const ull& _cells_per_col) noexcept
{
    return make_pair(((_cell / _SIZE) / 2) * 2, ((_cell % _SIZE) / 2) * 2);
}
// ------------------------------------------------------------

/** @brief Prints the board in a nice way, works better for numbers of max 2 digits
 */
void print(const vector<vector<ull>>& _board) noexcept
{
    string line; 
    for (ull i = 0; i < _board.size() * 4; ++i) line += "-";
    cout << line << endl;
    for (ull row = 0; row < _board.size(); ++row)
    {
        for (ull col = 0; col < _board.size(); ++col)
        {
            cout << _board[row][col];
            if (to_string(_board[row][col]).length() == 2) cout << "| ";   
            else cout << " | ";
        }   
        cout << endl << line << endl;
    }
}

/** 
 * @brief Checks if the given deleted cell belongs to given sub-quadrant 
 * 
 * @param _size: the size of the current sub-quadrant  
 * @param _row: the initial row of the current sub-quadrant
 * @param _col: the initial col of the current sub-quadrant
 * @param _del_row: the row of the deleted cell
 * @param _del_col: the col of the deleted cell
 *
 * @return true if the deleted cell belongs to the current sub-quadrant, false otherwise
 */
bool contains_cell(ull _size, ull _row, ull _col, ull _del_row, ull _del_col) noexcept
{
    bool belongs = false;
    if (_del_row < _row + _size && _del_row >= _row &&
        _del_col < _col + _size && _del_col >= _col) 
        belongs = true;

    return belongs;
}

/*
 * # PROOF OF CORRECTNESS (MATHEMATICAL INDUCTION) 
 *
 * ## Claim: A 2^n x 2^n board with any one cell removed can always be tiled with L-trominoes.
 *
 * ## Base case (n=1): A 2x2 board with one cell removed has exactly 3 cells left,
 * which form exactly one L-tromino. 
 *
 * ## Inductive step: Assume any 2^k x 2^k board with one cell removed can be tiled (hypothesis).
 * We want to prove it also holds for a 2^(k+1) x 2^(k+1) board with one cell removed.
 *   1. Divide the board into 4 quadrants of size 2^k x 2^k.
 *   2. Exactly one quadrant contains the removed cell.
 *   3. Place one L-tromino at the center, covering the inner corner of the other 3 quadrants.
 *   4. Now each of the 4 quadrants has exactly one "missing" cell:
 *        - the original removed cell  (for the quadrant that contained it), or
 *        - the inner corner covered by the center tromino (for the remaining 3).
 *   5. By the inductive hypothesis, each of the 4 sub-boards can be tiled independently. 
 *
 * The 4x4 case falls out for free from applying the inductive step to the base case —
 * there is no need to prove it separately.
 *
 * ## COMPLEXITY ANALYSIS 
 *
 * Let N = total cells = 2^n x 2^n = 4^n  (where n is the user input, the board power).
 *
 * Time: O(4^n) = O(N)  — linear in the number of cells.
 *   Each cell is written exactly once (as part of a tromino or marked as the deleted cell).
 *   Recurrence: T(k) = 4*T(k/2) + O(1), where k is the side length.
 *   Master Theorem case 1 (a=4, b=2, log_b(a)=2 > 0): T(k) = O(k^2).
 *   Substituting k = 2^n: T = O((2^n)^2) = O(4^n).
 *
 * Space: O(4^n)
 *   - Board storage:   O(4^n) for the 2^n x 2^n grid.
 *   - Recursion stack: O(n) frames (one per level of division), negligible vs. the board.
 *   Total dominated by the board: O(4^n).
 */

/** @brief Solves the tromino tiling problem for a sub-quadrant.
 * Invariant: _del_row/_del_col is always the missing cell of THIS sub-quadrant.
 * If a child contains that missing cell, it inherits it as its own _del.
 * Otherwise the child gets the center corner as its _del (to be filled later by the center tromino).
 *
 *     X X | X X   (0 = missing cell, D = center corners reserved for the center tromino)
 *     0 x | D X
 *     ----------
 *     X D | D X
 *     X X | X X
 *
 * @param _size:     side length of the current sub-quadrant
 * @param _row:      top-left row of the current sub-quadrant
 * @param _col:      top-left col of the current sub-quadrant
 * @param _del_row:  row of the missing cell for this sub-quadrant
 * @param _del_col:  col of the missing cell for this sub-quadrant
 */
void solve(vector<vector<ull>>& _board, ull _size, ull _row, ull _col, ull _del_row, ull _del_col) noexcept
{   
    // Base case
    if (_size == 2)
    {
        // If cointains the general deleted cell, then mark the rest of the cells with the current trominoe
        if (contains_cell(_size, _row, _col, DEL_ROW, DEL_COL))
        {
            _board[_row][_col] = CURRENT_TROMINOE;
            _board[_row][_col+1] = CURRENT_TROMINOE;
            _board[_row+1][_col] = CURRENT_TROMINOE;
            _board[_row+1][_col+1] = CURRENT_TROMINOE;
            _board[DEL_ROW][DEL_COL] = 0;  // mark the deleted cell of the board
            CURRENT_TROMINOE++;
            return;    
        }
        // If not contains any deleted cell, then mark the rest of the cells with the current trominoe except for the _del_row/col
        _board[_row][_col] = CURRENT_TROMINOE;
        _board[_row][_col+1] = CURRENT_TROMINOE;
        _board[_row+1][_col] = CURRENT_TROMINOE;
        _board[_row+1][_col+1] = CURRENT_TROMINOE;
        _board[_del_row][_del_col] = 0;  // mark the deleted cell of the board
        CURRENT_TROMINOE++;
        return;
    }

    ull next_size = _size / 2;
  
    ull init_del_row = _row + next_size - 1;
    ull init_del_col = _col + next_size - 1; 

    // Checks if the childs of this sub-quadrant contains his deleted cell, if so, then the child will have to use it as a deleted cell
    // because is like repecting the cell of the father
    bool contains_del_cell_up_left = contains_cell(next_size, _row, _col, _del_row, _del_col);
    bool contains_del_cell_up_right = contains_cell(next_size, _row, _col + next_size, _del_row, _del_col);
    bool contains_del_cell_down_left = contains_cell(next_size, _row + next_size, _col, _del_row, _del_col);
    bool contains_del_cell_down_right = contains_cell(next_size, _row + next_size, _col + next_size, _del_row, _del_col);

    // up and left
    if (contains_del_cell_up_left)
        solve(_board, next_size, _row, _col, _del_row, _del_col);                      
    else 
        solve(_board, next_size, _row, _col, init_del_row, init_del_col);                      

    // up and right
    if (contains_del_cell_up_right)
        solve(_board, next_size, _row, _col + next_size, _del_row, _del_col);                      
    else 
        solve(_board, next_size, _row, _col + next_size, init_del_row, init_del_col+1);

    // down and left
    if (contains_del_cell_down_left)
        solve(_board, next_size, _row + next_size, _col, _del_row, _del_col);                      
    else 
        solve(_board, next_size, _row + next_size, _col, init_del_row+1, init_del_col);

    // down and right
    if (contains_del_cell_down_right)
        solve(_board, next_size, _row + next_size, _col + next_size, _del_row, _del_col);
    else 
        solve(_board, next_size, _row + next_size, _col + next_size, init_del_row+1, init_del_col+1);

    // now treat the center as a small board of 2*2, but just if they haven't been marked before
    if (_board[init_del_row][init_del_col] == 0) _board[init_del_row][init_del_col] = CURRENT_TROMINOE;
    if (_board[init_del_row][init_del_col + 1] == 0) _board[init_del_row][init_del_col + 1] = CURRENT_TROMINOE;
    if (_board[init_del_row + 1][init_del_col] == 0) _board[init_del_row + 1][init_del_col] = CURRENT_TROMINOE;
    if (_board[init_del_row + 1][init_del_col + 1] == 0) _board[init_del_row + 1][init_del_col + 1] = CURRENT_TROMINOE;
    _board[DEL_ROW][DEL_COL] = 0;  // mark the deleted cell of the board
    CURRENT_TROMINOE++;
}

/*
Example for n = 4 and cell = 44

4
44

Result: 

Quadrant of pair : 0 12
Quadrant of pair2 : 2 12
cells: 256      row :2    col : 12
Board:
-------------- - --------------- - --------------- - -----------------
1 | 1 | 2 | 2  |  6 | 6 | 7 | 7  |  22| 22| 23| 23 |  27| 27| 28| 28| 
-------------- - --------------- - --------------- - -----------------
1 | 5 | 5 | 2  |  6 | 10| 10| 7  |  22| 26| 26| 23 |  27| 31| 31| 28| 
-------------- - --------------- - --------------- - -----------------
3 | 5 | 4 | 4  |  8 | 8 | 10| 9  |  24| 26| 25| 25 |  0 | 29| 31| 30| 
-------------- - --------------- - --------------- - -----------------
3 | 3 | 4 | 21 |  21| 8 | 9 | 9  |  24| 24| 25| 42 |  29| 29| 30| 30| 

-------------- - --------------- - --------------- - -----------------

11| 11| 12| 21 |  16| 16| 17| 17 |  32| 32| 33| 42 |  42| 37| 38| 38| 
-------------- - --------------- - --------------- - -----------------
11| 15| 12| 12 |  16| 20| 20| 17 |  32| 36| 33| 33 |  37| 37| 41| 38| 
-------------- - --------------- - --------------- - -----------------
13| 15| 15| 14 |  18| 20| 19| 19 |  34| 36| 36| 35 |  39| 41| 41| 40| 
-------------- - --------------- - --------------- - -----------------
13| 13| 14| 14 |  18| 18| 19| 85 |  34| 34| 35| 35 |  39| 39| 40| 40| 

-------------- - --------------- - --------------- - -----------------

43| 43| 44| 44 |  48| 48| 49| 85 |  85| 64| 65| 65 |  69| 69| 70| 70| 
-------------- - --------------- - --------------- - -----------------
43| 47| 47| 44 |  48| 52| 49| 49 |  64| 64| 68| 65 |  69| 73| 73| 70| 
-------------- - --------------- - --------------- - -----------------
45| 47| 46| 46 |  50| 52| 52| 51 |  66| 68| 68| 67 |  71| 71| 73| 72| 
-------------- - --------------- - --------------- - -----------------
45| 45| 46| 63 |  50| 50| 51| 51 |  66| 66| 67| 67 |  84| 71| 72| 72| 

-------------- - --------------- - --------------- - -----------------

53| 53| 54| 63 |  63| 58| 59| 59 |  74| 74| 75| 84 |  84| 79| 80| 80| 
-------------- - --------------- - --------------- - -----------------
53| 57| 54| 54 |  58| 58| 62| 59 |  74| 78| 75| 75 |  79| 79| 83| 80| 
-------------- - --------------- - --------------- - -----------------
55| 57| 57| 56 |  60| 62| 62| 61 |  76| 78| 78| 77 |  81| 83| 83| 82| 
-------------- - --------------- - --------------- - -----------------
55| 55| 56| 56 |  60| 60| 61| 61 |  76| 76| 77| 77 |  81| 81| 82| 82| 
-------------- - --------------- - --------------- - -----------------

*/

int main()
{
    ll n; cin >> n;        // power of n
    ll cell; cin >> cell;  // number of the cell to delete, from [0...+]
    if (n < 1) { cout << "No valid n : " << n << endl; return 0;}
    if (cell < 0) { cout << "No valid deleted cell : " << cell << endl; return 0;}

    _SIZE = pow(2, n);      // number of cells per column and row
    ull cells = _SIZE * _SIZE;       // Total
    DEL_ROW = cell/_SIZE;        DEL_COL = cell % _SIZE;   

    vector<vector<ull>> board(_SIZE, vector<ull>(_SIZE));
    board[DEL_ROW][DEL_COL] = 0;  // mark the deleted cell of the board

    // <debug>
    auto pr = quadrant_of(DEL_ROW, DEL_COL);
    auto pr2 = quadrant_of_cell(cell, _SIZE);

    cout << "Quadrant of pair : " << pr.first << " " << pr.second << endl;
    cout << "Quadrant of pair2 : " << pr2.first << " " << pr2.second << endl;
    cout << "cells: " << cells << "      row :" << DEL_ROW << "    col : " << DEL_COL << endl; 
    cout << "Board: " << endl << endl;
    // </debug>

    solve(board, _SIZE, 0, 0, DEL_ROW, DEL_COL);
    print(board);

    return 0;
}