#pragma once                                                                                                                  
#include <chrono>

namespace chronometer
{                                                                                                        
    using clock = std::chrono::high_resolution_clock;                                                                         
    static clock::time_point _start;                                                                                          
                                                                                                                               
    inline void clock_start() 
    {
        _start = clock::now(); 
    }                                                                            

    inline long long clock_end() 
    {                                                                                                   
        return std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - _start).count();                          
    }

    // same as clock_end but in microseconds, for the things that take less than 1 ms
    inline long long clock_end_us()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - _start).count();
    }                                                                                                                             
}