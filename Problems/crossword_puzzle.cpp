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

// returns the vector of strings splited, the min lenght and the max lenght of the words
tuple<vector<string>, ull, ull> split(const string& _str, const char _c) noexcept 
{
    vector<string> words;
    string next_str;
    ull _min = oo, _max = 0; 
    for (ull i = 0; i < _str.size(); ++i)
    {
        if (_str[i] == _c) 
        {
            words.push_back(next_str);
            _min = min(next_str.size(), _min);
            _max = max(next_str.size(), _max);
            next_str = ""; 
            continue;
        }
        next_str += _str[i];
    }
    if (!next_str.empty()) // push the last string that doesn't have ';'
    {
        words.push_back(next_str); 
        _min = min(next_str.size(), _min);
        _max = max(next_str.size(), _max);
    }
    return make_tuple(words, _min, _max);
}

class Spot
{
    public:
        ull row{oo}; 
        ull col{oo};
        ull size{0};
        bool is_vertical{false};
        
        /** @brief tries to place the given _word in the _crossword using  
         * @returns true, if it was completely added, otherwise false
         */ 
        bool place(vector<string>& _crossword, string& _word) const noexcept
        {
            if (_word.size() != size) return false;

            for (ull i = row, j = col, k = 0; k < size; ++k)
            {
                if (_crossword[i][j] == '-') // if it's an empty space just asing it
                    _crossword[i][j] = _word[k];
                else if (_crossword[i][j] != _word[k]) // if it's not the same letter COLLIDES
                {
                    // remove what we have done
                    for (ull ii = row, jj = col, kk = 0; kk < k; ++kk)
                    {
                        if (is_vertical)
                        {
                            // if the right and left cell are '+' or '-' then we can remove the letter, 
                            // otherwise we have to keep it because it is part of another perpendicular word
                            bool left_ok  = (jj == 0 || _crossword[ii][jj-1] == '+' || _crossword[ii][jj-1] == '-');
                            bool right_ok = (jj+1 >= _crossword[ii].size() || _crossword[ii][jj+1] == '+' || _crossword[ii][jj+1] == '-');
                            if (left_ok && right_ok) _crossword[ii][jj] = '-';
                            ++ii;
                        }
                        else 
                        {
                            // same as above but for horizontal
                            bool up_ok   = (ii == 0 || _crossword[ii-1][jj] == '+' || _crossword[ii-1][jj] == '-');
                            bool down_ok = (ii+1 >= _crossword.size() || _crossword[ii+1][jj] == '+' || _crossword[ii+1][jj] == '-');
                            if (up_ok && down_ok) _crossword[ii][jj] = '-';
                            ++jj;
                        }
                    }
                    return false;
                }

                if (is_vertical) ++i;
                else ++j;
            }

            return true;
        }

        void clear(vector<string>& _crossword) const noexcept
        {
            for (ull i = row, j = col, it = 0; it < size; ++it)
            {
                if (is_vertical)
                {
                    bool left_ok  = (j == 0 || _crossword[i][j-1] == '+' || _crossword[i][j-1] == '-');
                    bool right_ok = (j+1 >= _crossword[i].size() || _crossword[i][j+1] == '+' || _crossword[i][j+1] == '-');
                    if (left_ok && right_ok) _crossword[i][j] = '-';
                    ++i;
                }
                else 
                {
                    bool up_ok   = (i == 0 || _crossword[i-1][j] == '+' || _crossword[i-1][j] == '-');
                    bool down_ok = (i+1 >= _crossword.size() || _crossword[i+1][j] == '+' || _crossword[i+1][j] == '-');
                    if (up_ok && down_ok) _crossword[i][j] = '-';
                    ++j;
                }
            }
        }

        void set_coord(const ull _row, ull _col) noexcept
        {
            this->row = _row; 
            this->col = _col; 
        }
};

struct SpotEqual
{
    bool operator() (const Spot& _a, const Spot& _b) const noexcept
    {
        if (_a.row == _b.row && 
            _a.col == _b.col && 
            _a.is_vertical == _b.is_vertical && 
            _a.size == _b.size)
            return true;
        else return false;
    }   
};

struct SpotHash
{
    ull operator() (const Spot& _spot) const noexcept
    {                     // the number of the cell as a contiguos array
        return hash<ull>{}(_spot.row*_spot.col + _spot.row);
    }
};

vector<string> solve(vector<string>& _cw, string& _words) noexcept
{
    // 1. split the words that are separated by ';', and get the min and max lenght for the words
    auto tuple = split(_words, ';');
    vector<string> words = get<0>(tuple);
    ull min_word = get<1>(tuple);
    ull max_word = get<2>(tuple);
   
    // 2. look for all the spots 
    unordered_set<Spot, SpotHash, SpotEqual> spots_set;
    for (ull i = 0; i < _cw.size(); ++i) // goes to every cell and check for the first '-'
    {
        for (ull j = 0; j < _cw[i].size(); ++j) 
        {
            Spot hor_spot, ver_spot;   ver_spot.is_vertical = true;
            // goes up-down or right-left to look for the whole spot
            // and adds it to the sports_cords if doesnt exists 
            if (_cw[i][j] == '-') 
            {
                // first try to fill vertical
                for (ll k = i; k >= 0 && k < _cw.size() && _cw[k][j] == '-'; --k) // goes up
                {
                    ++ver_spot.size;
                    ver_spot.set_coord(k, j); // update the initial coord
                }
                for (ull k = i+1; k >= 0 && k < _cw.size() && _cw[k][j] == '-'; ++k) // goes down
                    ++ver_spot.size;

                // then try to fill horizontal
                for (ll k = j; k >= 0 && k < _cw[0].size() && _cw[i][k] == '-'; --k) // goes left
                {
                    ++hor_spot.size;
                    hor_spot.set_coord(i, k); // update the initial coord
                }
                for (ull k = j+1; k >= 0 && k < _cw.size() && _cw[i][k] == '-'; ++k) // goes right
                    ++hor_spot.size;

                // finally add them to the set if they could hold a word 
                if (ver_spot.size >= min_word && ver_spot.size <= max_word)
                    spots_set.insert(ver_spot);
                if (hor_spot.size >= min_word && hor_spot.size <= max_word)
                    spots_set.insert(hor_spot);
            } 
        }
    }


    // 3. then as we only have a 10x10 crossword we would do brute force with backtracking trying to 
    // place all the words in the given spots till we find the first solution 
    
    ull s_indx = 0;
    vector<Spot> spots_vec;                                  // from s_indx to back all the spots are filled
    vector<bool> words_added(words.size(), false);  // the know quickly if we have added a word
    for (auto spot : spots_set)
        spots_vec.push_back(spot); 

    stack<ull> last_word_indx;      // to simulate the call stack, this holds the indx from words[indx] of last word tried 
    last_word_indx.push(0);      // the first index starts at 0
    while (s_indx < words.size())
    {
        Spot spot = spots_vec[s_indx];
            
        // for every spot we try to place every word
        bool has_placed = false;
        ull i = last_word_indx.top(); last_word_indx.pop();
        for (; i < words.size(); ++i)
        {
            if (words_added[i]) continue; 
            string word = words[i];
            
            if (spot.place(_cw, word)) // if it was correctly placed
            {
                last_word_indx.push(i+1); // we tried till i word, so if we come back, our next will be i+1
                last_word_indx.push(0);   // and the next spot will start at 0
                ++s_indx;                   // next spot
                words_added[i] = true;      
                has_placed = true;   
                break;
            }
        }
        if (has_placed) continue;
        
        // if we haven't found any word for this spot means is not the correct branch
        --s_indx;                                 // previous spot
        spots_vec[s_indx].clear(_cw); // clear the previous spot
        words_added[last_word_indx.top() - 1] = false; // enable the last used word
    }

    return _cw;
}

int main()
{
    io_boost;                          
                                                                 // separated by ;
    ull size_of_cw = 10;      vector<string> crossword(size_of_cw);    string words;         
    for (ull i = 0; i < size_of_cw; ++i) cin >> crossword[i];
    cin >> words;

    // <checkers> not needed if we guaranteed a competitive input: e.g: is not 10x10 crossword
    bool is_bad = false; 
    if (crossword.size() != size_of_cw) is_bad = true;
    for (int i = 0; i < crossword.size(); ++i) if (crossword[i].size() != crossword.size()) is_bad = true;
    if (is_bad) { cout << "Not a " << size_of_cw << "x" << size_of_cw << " crossword" << endl;   return 0; }
    // </checkers>

    crossword = solve(crossword, words);

    for (ull i = 0; i < crossword.size(); ++i)
    {
        for (ull j = 0; j < crossword[i].size(); ++j)
            cout << crossword[i][j];
        cout << endl;
    }

    return 0;
}

/** @example
+-++++++++
+-++++++++
+-++++++++
+-----++++
+-+++-++++
+-+++-++++
+++++-++++
++------++
+++++-++++
+++++-++++
LONDON;DELHI;ICELAND;ANKARA


+-++++++++
+-++++++++
+-------++
+-++++++++
+-++++++++
+------+++
+-+++-++++
+++++-++++
+++++-++++
++++++++++
AGRA;NORWAY;ENGLAND;GWALIOR


++++++-+++
++------++
++++++-+++
++++++-+++
+++------+
++++++-+-+
++++++-+-+
++++++++-+
++++++++-+
++++++++-+
ICELAND;MEXICO;PANAMA;ALMATY


++++++++++
+------+++
+++-++++++
+++-++++++
+++-----++
+++-++-+++
++++++-+++
++++++-+++
++++++-+++
++++++++++
POLAND;LHASA;SPAIN;INDIA


+-++++++++
+-------++
+-++-+++++
+-------++
+-++-++++-
+-++-++++-
+-++------
+++++++++-
++++++++++
++++++++++
ANDAMAN;MANIPUR;ICELAND;ALLEPY;YANGON;PUNE
*/