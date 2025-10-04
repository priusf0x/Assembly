#include "tools.h"

#include <stdio.h>

#include "Assert.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "color.h"

void* recalloc(void*  pointer,
               size_t current_size,
               size_t new_size)
{
    ASSERT(pointer == NULL);

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

size_t
CountCharInStr(char        character,
               const char* str)
{
    ASSERT(str != NULL);

    const char* pointer_to_char = str;
    size_t count = 0;

    while (*pointer_to_char != '\0')
    {
        pointer_to_char = strchr(pointer_to_char, character);
        if (pointer_to_char == NULL)
        {
            break;
        }
        count++;
        pointer_to_char++;
    }

    return count;
}

char*
SkipSpaces(char* string)
{
    ASSERT(string != NULL);

    char character = *string;

    while (isspace(character) && (character != '\0'))
    {
        string++;
        character = *string;
    }

    return string;
}

char*
SkipNotSpaces(char* string)
{
    ASSERT(string != NULL);

    char character = *string;

    while (!isspace(character) && (character != '\0'))
    {
        string++;
        character = *string;
    }

    return string;
}

bool
IsStrNum(char* string)
{
    int count = int(SkipNotSpaces(string) - string);
    for (int index = 0; index < count; index++)
    {
        if (!isdigit(string[index]))
            return false;
    }

    return true;
}


