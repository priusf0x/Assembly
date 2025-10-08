#ifndef DISASSEMBLER_COMMANDS_H
#define DISASSEMBLER_COMMANDS_H

#include <stdlib.h>
#include <stdio.h>

void PrintPushArg(int* instructions, size_t* command_index, FILE* output);
void PrintPopArg(int* instructions, size_t* command_index, FILE* output);

enum  disassembler_commands_e
{
    DISASSEMBLER_COMMAND_HLT          = 0,
    DISASSEMBLER_COMMAND_PUSH         = 1,
    DISASSEMBLER_COMMAND_OUT          = 2,
    DISASSEMBLER_COMMAND_ADD          = 3,
    DISASSEMBLER_COMMAND_SUB          = 4,
    DISASSEMBLER_COMMAND_MUL          = 5,
    DISASSEMBLER_COMMAND_DIV          = 6,
    DISASSEMBLER_COMMAND_PUSH_IN_REG  = 7,
    DISASSEMBLER_COMMAND_POP          = 8,
    DISASSEMBLER_COMMAND_SQRT         = 9,
    DISASSEMBLER_COMMAND_IN           = 10
};

struct disassembler_command_t
{
    const char* command_name;
    const enum disassembler_commands_e return_value;
    void (*binary_handler)(int* instructions, size_t* command_index, FILE* output);
};

const struct disassembler_command_t DISASSEMBLER_COMMANDS_ARRAY[] = {
    {.command_name = "HLT",  .return_value = DISASSEMBLER_COMMAND_HLT,         .binary_handler = NULL        },
    {.command_name = "PUSH", .return_value = DISASSEMBLER_COMMAND_PUSH,        .binary_handler = PrintPushArg},
    {.command_name = "OUT",  .return_value = DISASSEMBLER_COMMAND_OUT,         .binary_handler = NULL        },
    {.command_name = "ADD",  .return_value = DISASSEMBLER_COMMAND_ADD,         .binary_handler = NULL        },
    {.command_name = "SUB",  .return_value = DISASSEMBLER_COMMAND_SUB,         .binary_handler = NULL        },
    {.command_name = "MUL",  .return_value = DISASSEMBLER_COMMAND_MUL,         .binary_handler = NULL        },
    {.command_name = "DIV",  .return_value = DISASSEMBLER_COMMAND_DIV,         .binary_handler = NULL        },
    {.command_name = "PUSH", .return_value = DISASSEMBLER_COMMAND_PUSH_IN_REG, .binary_handler = PrintPushArg},
    {.command_name = "POP",  .return_value = DISASSEMBLER_COMMAND_POP,         .binary_handler = PrintPopArg },
    {.command_name = "SQRT", .return_value = DISASSEMBLER_COMMAND_SQRT,        .binary_handler = NULL        },
    {.command_name = "DIV",  .return_value = DISASSEMBLER_COMMAND_IN,          .binary_handler = NULL        }};
const size_t DISASSEMBLER_COMMANDS_COUNT = sizeof(DISASSEMBLER_COMMANDS_ARRAY) / sizeof(DISASSEMBLER_COMMANDS_ARRAY[0]);

const char* const PROCESSORS_REG[] =
{
    "R0X",
    "RAX",
    "RBX",
    "RCX",
    "RDX"
};
const int PROCESSOR_REG_COUNT = sizeof(PROCESSORS_REG) /  sizeof(PROCESSORS_REG[0]);

#endif //DISASSEMBLER_COMMANDS_H
