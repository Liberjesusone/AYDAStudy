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
 * Time limit: 1.00 s
 * Memory limit: 512 MB
 * 
 * There are n people who want to get to the top of a building which has only one elevator. 
 * You know the weight of each person and the maximum allowed weight in the elevator. What 
 * is the minimum number of elevator rides?
 * 
 * # Input
 * The first input line has two integers n and x: the number of people and the maximum allowed 
 * weight in the elevator.
 * 
 * # The second line has n integers w_1,w_2,...,w_n: the weight of each person.
 *
 * # Output
 * Print one integer: the minimum number of rides.
 *
 * # Constraints
 * 1 <= n <= 20
 * 1 <= x <= 10^9 = 1 000 000 000
 * 1 <= w_i <= x 
 * 
 * # Example
 * Input:
 * 4 10
 * 4 8 6 1
 * 
 * Output:
 * 2
 */
int main()
{
    io_boost;

    ull n; ull load; cin >> n; cin >> load;
    vector<ull> weights(n);
    unordered_set<ull> rides;
    for (ull i = 0; i < n; ++i) cin >> weights[i];

    // for dp[i] means the minimun amount of rides as .first and the weight of the last ride as .second
    // i would be a number that its bit representation will represent the subset of people that are in the elevator
    // 101 000 means that the first person and the third one will be taken into acount to calculate
                            // 1 << n = 1 * 2^n
    vector<pair<ull, ull>> dp(1 << n, {oo, 0});
    dp[0] = {1, 0};
                            // all bits in 1
    for (ull mask = 1; mask <= (1 << n) - 1; ++mask) 
    {
        // 1. Iterate for all the people in this bit mask, and take a look at the trip configuration without them for every person
        // the trip configurations that has the least amount of trips or the least amount of accumulated weigth will be the one
        // that we will use to insert again the person that we take away. because this trip config will leaves more available space later 

        /* E.g: for load = 10   and weigts are 7, 5, 4
           for 111 -> we look at the trip config of 011, 101 and 110, and the best one will be selected, imagine that 

           011 -> {1, 9}   So the better config is 011 in this case we have less trips, and then we insert the person that took away
           101 -> {2, 4}   the one with weight = 7, and that will lead us to 111 -> {2, 7}, if we select the other ones, we will end up 
           110 -> {2, 5}   with 111 -> {2, 9} that is a worse trip config, because our last trip leaves us with less space
         */

        pair<ull, ull> _min = {oo, oo};  // minimun amount of trips and acumulated weight found 
        ull m = mask;                         // m = mask to consume as an iterator to obtain the 1 bits
        
        while (m)                                   // indx     = n, n-1, n-2, .... 3, 2, 1, 0
        {                                           // for mask = 0  1    0         1  0  1  0                             
            ull indx = countr_zero(m); // get the indx of the least significant 1 bit

            ull selected = ~(1ULL << indx);              // we get the selected bit 000 0010 and negate it = 111 1101
            ull mask_without_selected = mask & selected; // 010 1000
            
            pair<ull, ull> trip_config = dp[mask_without_selected];
            pair<ull, ull> new_trip = {trip_config.first, trip_config.second + weights[indx]};
            if (new_trip.second > load) // if the new trip accumulated weight exceeds the load  
            {
                ++new_trip.first;                // We sum a new trip
                new_trip.second = weights[indx]; // Accumulated weight is just the new weight
            }
            
            // We select the new min prioritizing the min amount of trips and then the accumulated weight
            _min = min(_min, new_trip);             

            m &= m - 1; // erase the least significant 1 bit, so next m = 010 1000
        }

        // 2. After found the min, update the dp
        dp[mask] = _min;     
        
        if (_min == make_pair(oo, oo)) // if not, but this may never occur 
        {
            dp[mask] = {oo, oo};
            throw std::runtime_error("== Minimun amount of trips and weight not found ==");
        }
    }
            // 2^n -1 = 111 111 .... 111 -> all n bits as 1
    cout << dp[(1 << n) - 1].first << endl;

    return 0;
}