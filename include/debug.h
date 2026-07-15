/**
 * Debug tools
 */

#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

/**
 * Disassmble bytecode into a human readable form
 */
void disassembleChunk(Chunk* chunk, const char* name);

/**
 * Disassemble an individual instruction
 */
size_t disassembleInstruction(Chunk* chunk, size_t offset);

#endif
