#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
    #define DEBUG_LOG(fmt, ...) printf("DEBUG: " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...) // No operation (NOP) if DEBUG is not defined
#endif

#endif