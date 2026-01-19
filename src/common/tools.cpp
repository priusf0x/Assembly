#include "tools.h"

#include <cassert>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "compiler_commands.h"
#include "common_commands.h"
#include "color.h"
#include "op_handler.h"

void* recalloc(void*  pointer,
               size_t current_size,
               size_t new_size)
{

    pointer = realloc(pointer, new_size);

    if (pointer == NULL)
    {
        return NULL;
    }

    memset((char*) pointer + current_size, 0,
                     new_size - current_size);

    return pointer;
}

size_t
CountCharInStr(char        character,
               const char* str)
{
    assert(str != NULL);

    const char* pointer_to_char = str;
    size_t count = 0;

    while (*pointer_to_char != '\0')
    {
        pointer_to_char = strchr(pointer_to_char,
                                     character);
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
    assert(string != NULL);

    char character = *string;

    while ((isspace(character)) 
                && (character != '\0'))
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
    assert(string != NULL);

    char character = *string;

    while (!isspace(character) && (character != '\0'))
    {
        string++;
        character = *string;
    }

    return string;
}

uint8_t
TranslateCommandOpcode(uint8_t* processor_instructions,
                       size_t*  instruction_number)
{
    // TODO:        read_command -> cmd_opcode

    uint8_t read_command = 0;
    uint8_t* pointer_to_command = processor_instructions + *instruction_number;

    if ((*(pointer_to_command) & ARGUMENT_SWITCH_MASK) != ARGUMENT_SWITCH_MASK)
    {
        read_command = *(pointer_to_command) >> 6;
    }
    else if ((*(pointer_to_command) & (EXTENDED_COMMAND_PACK)) != EXTENDED_COMMAND_PACK)
    {
        read_command = ((*(pointer_to_command) & (EXTENDED_COMMAND_PACK)) >> 3)
                        + 0b0000'0011;

    }
    else
    {

        read_command = (*pointer_to_command) & ARGUMENT_MASK;
        pointer_to_command += sizeof(uint8_t);
        *instruction_number += sizeof(uint8_t);
        read_command += (uint8_t) ((*(pointer_to_command) & ARGUMENT_SWITCH_MASK)) >> 3;
        read_command += 0b0000'1010;
    }

    return read_command;
}



