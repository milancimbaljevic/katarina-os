#pragma once

#include "print.h"

// #ifdef DEBUG_ENABLED
//     #define DEBUGLN(x) 
//         debugln x
//     #define DEBUG(x) 
//         debug x
// #else
//     #define DEBUGLN(x)
//     #define DEBUG(x)
// #endif


// #define FUNCTION_ENTER(x) 
//         debug("FUNCTION_ENTER: "); 
//         DEBUGLN(x)

#ifdef DEBUG_ENABLED
    #define DEBUGLN(x) \
        println x
    #define DEBUG(x) \
        print x
#else
    #define DEBUGLN(x)
    #define DEBUG(x)
#endif


#ifdef DEBUG_ENABLED
    #define FUNCTION_ENTER(x) \
            print("FUNCTION_ENTER: "); \
            DEBUGLN(x)
#else
    #define FUNCTION_ENTER(x)
#endif

