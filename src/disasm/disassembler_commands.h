#ifndef DISASSEMBLER_COMMANDS_H
#define DISASSEMBLER_COMMANDS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "common_commands.h"

void PrintHLT(uint8_t* instructions, size_t* command_index, FILE* output);
void PrintPush(uint8_t* instructions, size_t* command_index, FILE* output);
void PrintOut(uint8_t* instructions, size_t* command_index, FILE* output);
void PrintOperation(uint8_t* instructions, size_t* command_index, FILE* output);
void PrintPop(uint8_t* instructions, size_t* command_index, FILE* output);
void PrintJump(uint8_t* instructions, size_t* command_index, FILE* output);
void PrintCall(uint8_t* instructions, size_t* command_index, FILE* output);
void PrintDraw(uint8_t* instructions, size_t* command_index, FILE* output);

struct disassemble_command_t
{
    const enum commands_e binary_value;
    void (*binary_handler)(uint8_t* instructions, size_t* command_index, FILE* output);
};

const struct disassemble_command_t DISASSEMBLER_COMMANDS_ARRAY[] =
{
    {.binary_value = COMMAND_HLT,       .binary_handler = PrintHLT      },
    {.binary_value = COMMAND_PUSH,      .binary_handler = PrintPush     },
    {.binary_value = COMMAND_POP,       .binary_handler = PrintPop      },
    {.binary_value = COMMAND_OUT,       .binary_handler = PrintOut      },
    {.binary_value = COMMAND_OPERATION, .binary_handler = PrintOperation},
    {.binary_value = COMMAND_JMP,       .binary_handler = PrintJump     },
    {.binary_value = COMMAND_CALL,      .binary_handler = PrintCall     },
    {.binary_value = COMMAND_EMPTY_1,   .binary_handler = NULL          },
    {.binary_value = COMMAND_EMPTY_2,   .binary_handler = NULL          },
    {.binary_value = COMMAND_EMPTY_3,   .binary_handler = NULL          },
    {.binary_value = COMMAND_DRAW,      .binary_handler = PrintDraw     },

};

const size_t DISASSEMBLER_COMMANDS_COUNT = sizeof(DISASSEMBLER_COMMANDS_ARRAY) / sizeof(DISASSEMBLER_COMMANDS_ARRAY[0]);

#endif //DISASSEMBLER_COMMANDS_H
