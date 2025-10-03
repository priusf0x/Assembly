#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include <string.h>

size_t CountCharInStr(char character, const char* str);
void*  recalloc(void*  pointer, size_t current_size, size_t new_size);
void   PrintHelloMessage();

#endif //TOOLS_H
