#include "tools.h"

#include <stdio.h>

#include "stdlib.h"
#include "string.h"
#include "color.h"

void* recalloc(void*  pointer,
               size_t current_size,
               size_t new_size)
{
    pointer = realloc(pointer, new_size);
    memset((char*) pointer + current_size, 0, new_size - current_size);

    return pointer;
}

void PrintHelloMessage()
{
    printf(WHITE"    ▄████▄   ▄▄▄       ██▓     ▄████▄  \n"
                "   ▒██▀ ▀█  ▒████▄    ▓██▒    ▒██▀ ▀█  \n"
                "   ▒▓█    ▄ ▒██  ▀█▄  ▒██░    ▒▓█    ▄ \n"
                "   ▒▓▓▄ ▄██▒░██▄▄▄▄██ ▒██░    ▒▓▓▄ ▄██ \n"
                "   ▒ ▓███▀ ░ ▓█   ▓██▒░██████▒▒ ▓███▀  \n"
                "   ░ ░▒ ▒  ░ ▒▒   ▓▒█░░ ▒░▓  ░░ ░▒ ▒   \n"
                "     ░  ▒     ▒   ▒▒ ░░ ░ ▒  ░  ░  ▒   \n"
                "   ░          ░   ▒     ░ ░   ░        \n"
                "   ░ ░            ░  ░    ░  ░░ ░      \n");

    printf(RED  "                    █▄▓▄▄▓██▓▓█▄▄▄█▀███ \n"
                "   ▄▄▄█▓██▓▄▄▄██▓▓██ ▒░▓  ░░▒▒ ▓░▒░▒ ▒▒ \n"
                "    ░░ ░ ▓  ░ ░▒ ▒░░ ░ ▒  ░░░▒ ▒ ░ ▒  ▒ \n"
                "   ░ ░░  ░░ ░░ ░   ░ ░   ░ ░ ░ ░ ░  ░   \n"
                "    ░  ░   ░  ░       ░  ░  ░ ░         \n"
                "                           ░            \n"STANDARD);


}
