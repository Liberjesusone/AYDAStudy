#pragma once

namespace Module
{
    // Does O(log_2(exp)) iterations
    long long binpow(long long base, long long exp, long long mod = 1e9 + 7) noexcept
    {
        long long result = 1;
        base %= mod;
        while (exp > 0) // if exp = 0000000 binary 
        {
            if (exp & 1)    // if the less significant bit is 1
                result = result * base % mod;
            base = base * base % mod;
            exp >>= 1;      // right shift = divide by 2 -> displace one bit to right
        }
        return result;
    }

    long long addmod(long long a, long long b, long long mod = 1e9 + 7) noexcept
    {
        return (a % mod + b % mod) % mod;
    }

    long long submod(long long a, long long b, long long mod = 1e9 + 7) noexcept
    {
        return (a % mod - b % mod + mod) % mod;
    }

    long long multmod(long long a, long long b, long long mod = 1e9 + 7) noexcept
    {
        return ((a % mod) * (b % mod)) % mod;
    }

    // Fermat small theorem: b*b^{-1} = 1 mod m  ->   b^{-1} = b^{MOD - 2} % MOD       if MOD is prime 
    long long modinv(long long b, long long mod = 1e9 + 7) noexcept
    {
        return binpow(b, mod - 2, mod);
    }

    long long divmod(long long num, long long div, long long mod = 1e9 + 7) noexcept
    {
        return num % mod * modinv(div, mod) % mod;
    }

    const int MAX_FACT = 1e6 + 5; // 1 000 005
    long long fact[MAX_FACT], inv_fact[MAX_FACT];

    // Calculates all values for factorials and their modular inverses till 1 000 005 in the variables fact[], inv_fact[]
    void precompute(long long mod = 1e9 + 7)
    {
        fact[0] = 1;
        for (int i = 1; i < MAX_FACT; i++)
            fact[i] = fact[i-1] * i % mod;
        
        inv_fact[MAX_FACT-1] = modinv(fact[MAX_FACT-1], mod);
        for (int i = MAX_FACT-2; i >= 0; i--)
            inv_fact[i] = inv_fact[i+1] * (i+1) % mod;
    }

    // Combination: binomial coefficient
    long long C(long long n, long long k, long long mod = 1e9 + 7) 
    {
        if (k < 0 || k > n) return 0;
        return fact[n] % mod * inv_fact[k] % mod * inv_fact[n-k] % mod;
    }
}
using namespace Module;