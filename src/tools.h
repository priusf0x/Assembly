#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <string.h>

size_t CountCharInStr(char character, const char* str);
char*  SkipSpaces(char* string);
char*  SkipNotSpaces(char* string);
void*  recalloc(void*  pointer, size_t current_size, size_t new_size);
void   PrintHelloMessage();
bool   IsStrNum(char* string);

#endif //TOOLS_H
