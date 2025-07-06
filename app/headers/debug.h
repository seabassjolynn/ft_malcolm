#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
    #define IS_DEBUG_MODE 1
#else
    #define IS_DEBUG_MODE 0
#endif

#endif