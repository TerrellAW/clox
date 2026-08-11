/**
 * Common libraries needed throughout the program
 */

#ifndef CLOX_COMMON_H
#define CLOX_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Flags for activating debug logs
 */
// Prints bytecode
#define DEBUG_PRINT_CODE
// Prints bytecode with stack
#define DEBUG_TRACE_EXECUTION

/**
 * Max unsigned 8 bit integer size for data structures
 */
#define UINT8_COUNT (UINT8_MAX + 1)

#endif
