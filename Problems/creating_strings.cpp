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


/** @brief
 * In order to solve it we have to do all the possible permutations of the elements 
 * as we considerer every letter unique, if the string has n letters we would have 
 * n characters so we have n! possible permutations 
 *
 * In order to get them we can use backtracking to explore the whole tree of solutions with the
 * only condition of not going further when we have already taken all the letters of the string.
 * So we will use a stack so simulate the call stack and we will save there the indx of the current 
 * for where we stop at, to continue looking for the other possible solutions 
 */

/** @brief
 * Time limit: 1.00 s
 * Memory limit: 512 MB
 * 
 * Given a string, your task is to generate all different strings that can be created using its characters.
 * 
 * # Input
 * The only input line has a string of length n. Each character is between a–z.
 * 
 * # Output
 * First print an integer k: the number of strings. Then print k lines: the strings in alphabetical order.
 * 
 * # Constraints
 * 1 <= n <= 8
 * 
 * # Example
 * Input:
 * aabac
 * 
 * Output:
 * 20
 * aaabc
 * aaacb
 * aabac
 * aabca
 * aacab
 * aacba
 * abaac
 * abaca
 * abcaa
 * acaab
 * acaba
 * acbaa
 * baaac
 * baaca
 * bacaa
 * bcaaa
 * caaab
 * caaba
 * cabaa
 * cbaaa
 */
int main()
{
    io_boost;

    // We use an array that has the count of repetitions for every letter   a = 97; z = 122
    string word; cin >> word;

    int n = 26;   vector<int> letters(n);   
    for (auto let : word)
        ++letters[let - 'a']; // a is indx = 0;   z is indx 122-97 = 25
    
    // Print the amount of permutations for repeated elements 
    ull fac = 1; for (ull i = 2; i <= word.size(); ++i) fac *= i;   
    for (ull i = 0; i < letters.size(); ++i)  // for every letter 
    {
        ull c_div = 1;   // Calc. the factorial of the repetition count 
        for (ull c = 2; c <= letters[i]; ++c) c_div *= c;   
        fac /= c_div;
    }      
    cout << fac << endl;   

    stack<int> stack;
    stack.push(0); // indx of the first level
    string current_str = "";
    bool have_inserted = false;

    while (!stack.empty()) 
    {
        int top = stack.top(); stack.pop(); // we take the top indx
        have_inserted = false;
        
        if (current_str.size() == word.size())        // if we complete a possible word
        {
            cout << current_str << endl;      // print this permutation
            char last_letter = current_str[current_str.size()-1];
            ++letters[last_letter - 'a'];   // we add it again to use it later
            current_str.pop_back();
            ++stack.top();                  // in the next iteration we will try the next letter 
            continue;
        }
        
        for (int i = top; i < n; ++i) // for every letter
        {   
            int amount = letters[i];
            if (amount == 0) continue;
            
            char letter = i + 'a';   // current letter
            current_str += letter;
            
            have_inserted = true;
            --letters[i];             // we used one repetition
            stack.push(i);         // the indx where we left
            stack.push(0);         // the next indx always 0
            break;
        }
        
        if (have_inserted) continue;
        if (stack.empty()) break; 

        // if we have tried all the letters in this branch, we move to another the previous level 
        ++stack.top();     // to try the next letter from the one that we left
        auto last_letter = current_str[current_str.size()-1];
        ++letters[last_letter - 'a'];   // we add it again to use it later
        current_str.pop_back();
    }

    return 0;
}