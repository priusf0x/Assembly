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
#include "common_commands.h"

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

uint8_t
TranslateCommandNumber(uint8_t* processor_instructions,
                       size_t*  instruction_number)
{
    ASSERT(processor_instructions != NULL);
    ASSERT(instruction_number != NULL);
    uint8_t read_command = 0;
    uint8_t* pointer_to_command = processor_instructions + *instruction_number;

    if ((*(pointer_to_command) & ARGUMENT_SWITCH_MASK) ^ ARGUMENT_SWITCH_MASK)
    {
        read_command = *(pointer_to_command) >> 6;
    }
    else if ((*(pointer_to_command) & (EXTENDED_COMMAND_PACK)) ^ EXTENDED_COMMAND_PACK)
    {
        read_command = ((*(pointer_to_command) & (EXTENDED_COMMAND_PACK)) >> 3)
                        + 0b00000011;
    }
    else
    {
        read_command = (*pointer_to_command) & ARGUMENT_MASK;
        pointer_to_command += sizeof(uint8_t);
        *instruction_number += sizeof(uint8_t);
        read_command += (uint8_t) ((*(pointer_to_command) & ARGUMENT_SWITCH_MASK)) >> 3;
        read_command += 0b00001010;
    }

    return read_command;
}



