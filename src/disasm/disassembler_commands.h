#ifndef DISASSEMBLER_COMMANDS_H
#define DISASSEMBLER_COMMANDS_H

#include <stdlib.h>
#include <stdio.h>

#include "common_commands.h"

void PrintPushPopArg(int* instructions, size_t* command_index, FILE* output);
void PrintJumpArg(int* instructions, size_t* command_index, FILE* output);

struct disassemble_command_t
{
    const char* command_name;
    const enum commands_e return_value;
    void (*binary_handler)(int* instructions, size_t* command_index, FILE* output);
};

const struct disassemble_command_t DISASSEMBLER_COMMANDS_ARRAY[] = {
    {.command_name = "hlt",  .return_value = COMMAND_HLT,              .binary_handler = NULL           },
    {.command_name = "push", .return_value = COMMAND_PUSH,             .binary_handler = PrintPushPopArg},
    {.command_name = "out",  .return_value = COMMAND_OUT,              .binary_handler = NULL           },
    {.command_name = "add",  .return_value = COMMAND_ADD,              .binary_handler = NULL           },
    {.command_name = "sub",  .return_value = COMMAND_SUB,              .binary_handler = NULL           },
    {.command_name = "mul",  .return_value = COMMAND_MUL,              .binary_handler = NULL           },
    {.command_name = "div",  .return_value = COMMAND_DIV,              .binary_handler = NULL           },
    {.command_name = "push", .return_value = COMMAND_PUSH_FROM_REG,    .binary_handler = PrintPushPopArg},
    {.command_name = "pop",  .return_value = COMMAND_POP,              .binary_handler = PrintPushPopArg},
    {.command_name = "sqrt", .return_value = COMMAND_SQRT,             .binary_handler = NULL           },
    {.command_name = "in",   .return_value = COMMAND_IN,               .binary_handler = NULL           },
    {.command_name = "jmp",  .return_value = COMMAND_JMP,              .binary_handler = PrintJumpArg   },
    {.command_name = "ja",   .return_value = COMMAND_JA,               .binary_handler = PrintJumpArg   },
    {.command_name = "jae",  .return_value = COMMAND_JAE,              .binary_handler = PrintJumpArg   },
    {.command_name = "jb",   .return_value = COMMAND_JB,               .binary_handler = PrintJumpArg   },
    {.command_name = "jbe",  .return_value = COMMAND_JBE,              .binary_handler = PrintJumpArg   },
    {.command_name = "je",   .return_value = COMMAND_JE,               .binary_handler = PrintJumpArg   },
    {.command_name = "jne",  .return_value = COMMAND_JNE,              .binary_handler = PrintJumpArg   },
    {.command_name = "call", .return_value = COMMAND_CALL,             .binary_handler = PrintJumpArg   },
    {.command_name = "ret",  .return_value = COMMAND_RET,              .binary_handler = NULL           },
    {.command_name = "push", .return_value = COMMAND_PUSH_FROM_MEMORY, .binary_handler = PrintPushPopArg},
    {.command_name = "pop",  .return_value = COMMAND_POP_TO_MEMORY,    .binary_handler = PrintPushPopArg},
    {.command_name = "draw", .return_value = COMMAND_DRAW,             .binary_handler = NULL           }};
const size_t DISASSEMBLER_COMMANDS_COUNT = sizeof(DISASSEMBLER_COMMANDS_ARRAY) / sizeof(DISASSEMBLER_COMMANDS_ARRAY[0]);

#endif //DISASSEMBLER_COMMANDS_H
