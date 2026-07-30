#pragma once

#include <unordered_map>
#include <utility>

using ull = unsigned long long;
using ll = long long;


// ====================         ====================
// ==================== HASHES: ====================

struct CoordEqual
{
    bool operator() (const std::pair<ull, ull>& _a, const std::pair<ull, ull>& _b) const noexcept
    {
        if (_a.first == _b.first && 
            _a.second == _b.second)
            return true;
        else return false;
    }   
};

struct CoordHash
{
    ull operator() (const std::pair<ull, ull>& _pair) const noexcept
    {                     // the number of the cell as a contiguos array
        return std::hash<ull>{}(_pair.first*_pair.second + _pair.second);
    }
};

// ====================          ====================
// ==================== /HASHES: ====================