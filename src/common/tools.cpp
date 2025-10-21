#include "tools.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Assert.h"
#include "color.h"
#include "read_commands.h"
#include "compiler_commands.h"

void* recalloc(void*  pointer,
               size_t current_size,
               size_t new_size)
{
    ASSERT(pointer != NULL);

    pointer = realloc(pointer, new_size);
    memset((char*) pointer + current_size, 0, new_size - current_size);

    return pointer;
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

    while ((character == ' ') && (character != '\n') && (character != '\0'))
    {
        string++;
        character = *string;
    }
    if (character == '#')
    {
        while (character != '\n')
        {
            string++;
            character = *string;
        }
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
    int count = (int) (SkipNotSpaces(string) - string);
    for (int index = 0; index < count; index++)
    {
        if (!isdigit(string[index]))
            return false;
    }

    return true;
}

int
PutInstruction(uint8_t                         value,
               compiler_instructions_t* instructions)
{
    // printf("%d ", value);
    ASSERT(instructions != NULL);
    ASSERT(instructions->instructions_array != NULL);

    if (instructions->instructions_bytes_written >= instructions->instructions_max_bytes_amount - 10)
    {
        instructions->instructions_array = (uint8_t*) recalloc(instructions->instructions_array, instructions->instructions_max_bytes_amount, instructions->instructions_max_bytes_amount * 2);
        instructions->instructions_max_bytes_amount *= 2;
    }

    if (instructions->instructions_array == NULL)
    {
        return 1;
    }
    // fprintf(stderr, "%zu %zu\n", instructions->instructions_count, instructions->instructions_size);
    (instructions->instructions_array)[instructions->instructions_bytes_written] = value;
    instructions->instructions_bytes_written += sizeof(uint8_t);

    return 0;
}

int
PutInteger(int                      value,
           compiler_instructions_t* instructions)
{
    // printf("%d ", value);

    ASSERT(instructions != NULL);
    ASSERT(instructions->instructions_array != NULL);

    if (instructions->instructions_bytes_written >= instructions->instructions_max_bytes_amount - 10)
    {
        instructions->instructions_array = (uint8_t*) recalloc(instructions->instructions_array, instructions->instructions_max_bytes_amount, instructions->instructions_max_bytes_amount * 2);
        instructions->instructions_max_bytes_amount *= 2;
    }

    if (instructions->instructions_array == NULL)
    {
        return 1;
    }

    *((int*)(instructions->instructions_array + instructions->instructions_bytes_written)) = (int) value;
    instructions->instructions_bytes_written += sizeof(int);

    return 0;
}



