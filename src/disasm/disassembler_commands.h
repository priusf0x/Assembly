#ifndef DISASSEMBLER_COMMANDS_H
#define DISASSEMBLER_COMMANDS_H

#include <stdlib.h>
#include <stdio.h>

void PrintPushArg(int* instructions, size_t* command_index, FILE* output);

enum  commands_e
{
    COMMAND_HLT  = 0,
    COMMAND_PUSH = 1,
    COMMAND_OUT  = 2,
    COMMAND_ADD  = 3,
    COMMAND_SUB  = 4,
    COMMAND_MUL  = 5,
    COMMAND_DIV  = 6,
};

struct disassembler_command_t
{
    const char* command_name;
    const enum commands_e return_value;
    void (*binary_handler)(int* instructions, size_t* command_index, FILE* output);
};

const struct disassembler_command_t DISASSEMBLER_COMMANDS_ARRAY[] = {
    {.command_name = "HLT",   .return_value = COMMAND_HLT,               .binary_handler = NULL        },
    {.command_name = "PUSH",  .return_value = COMMAND_PUSH,              .binary_handler = PrintPushArg},
    {.command_name = "OUT",   .return_value = COMMAND_OUT,               .binary_handler = NULL        },
    {.command_name = "ADD",   .return_value = COMMAND_ADD,               .binary_handler = NULL        },
    {.command_name = "SUB",   .return_value = COMMAND_SUB,               .binary_handler = NULL        },
    {.command_name = "MUL",   .return_value = COMMAND_MUL,               .binary_handler = NULL        },
    {.command_name = "DIV",   .return_value = COMMAND_DIV,               .binary_handler = NULL        }};
const size_t commands_count = sizeof(DISASSEMBLER_COMMANDS_ARRAY) / sizeof(DISASSEMBLER_COMMANDS_ARRAY[0]);


#endif //DISASSEMBLER_COMMANDS_H
