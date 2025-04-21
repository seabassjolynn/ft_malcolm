#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
    #define DEBUG_LOG(fmt, ...) printf("DEBUG: " fmt "\n", ##__VA_ARGS__)
    #define IS_DEBUG_MODE 1
#else
    #define DEBUG_LOG(fmt, ...) // No operation (NOP) if DEBUG is not defined
    #define IS_DEBUG_MODE 0
#endif

#endif