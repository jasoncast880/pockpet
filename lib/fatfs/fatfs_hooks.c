#include "ff.h"

//test file for misc funcs
//
// Return time in packed format: 
// [31:25] = year from 1980 (7 bits)
// [24:21] = month (4 bits)
// [20:16] = day (5 bits)
// [15:11] = hour (5 bits)
// [10:5]  = minute (6 bits)
// [4:0]   = second/2 (5 bits)

DWORD get_fattime(void) {
    // Return a dummy time for now (e.g., Jan 1, 2025, 00:00:00)
    return ((DWORD)(2025 - 1980) << 25)  // Year 2025
         | ((DWORD)1 << 21)              // Month = Jan
         | ((DWORD)1 << 16)              // Day = 1
         | ((DWORD)0 << 11)              // Hour = 0
         | ((DWORD)0 << 5)               // Minute = 0
         | ((DWORD)0 >> 1);              // Second/2 = 0
}

