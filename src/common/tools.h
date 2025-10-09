#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <string.h>

#include "read_commands.h"
#include "compiler_commands.h"

size_t CountCharInStr(char character, const char* str);
char*  SkipSpaces(char* string);
char*  SkipNotSpaces(char* string);
void*  recalloc(void*  pointer, size_t current_size, size_t new_size);
bool   IsStrNum(char* string);
int    PutInstruction(int value, compiler_instructions_t* instructions);
void   FreeAll(compiler_instructions_t* instructions, char* input_buffer);

#endif //TOOLS_H
