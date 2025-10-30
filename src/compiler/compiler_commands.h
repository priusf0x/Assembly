#ifndef COMPILER_COMMANDS_H
#define COMPILER_COMMANDS_H

#include <stdlib.h>

#include "common_commands.h"
struct label_tabular_t;

enum label_instruction_return_e
{
    LABEL_INSTRUCTION_RETURN_SUCCESS,
    LABEL_INSTRUCTION_RETURN_MEMORY_ERROR,
    LABEL_INSTRUCTION_RETURN_INITIALIZATION_REPEAT,
    LABEL_INSTRUCTION_RETURN_EMPTY_NAME,
    LABEL_INSTRUCTION_RETURN_UNINITIALIZED_LABEL
};

struct compiler_instructions_t
{
    size_t           instructions_bytes_written;
    size_t           instructions_max_bytes_amount;
    uint8_t*         instructions_array;
    label_tabular_t* instructions_label_tabular;
};

enum  compiler_return_e
{
    COMPILER_RETURN_SUCCESS,
    COMPILER_RETURN_MEMORY_ERROR,
    COMPILER_RETURN_VALID_SYNTAX,
    COMPILER_RETURN_INCORRECT_COMMAND,
    COMPILER_RETURN_INVALID_SYNTAX,
    COMPILER_RETURN_EMPTY_COMMAND,
    COMPILER_RETURN_LABEL_ERROR
};

//=============== MAIN_COMMANDS ==================

compiler_return_e InitInstuctionStruct(compiler_instructions_t* instructions);
compiler_return_e TranslateCode(char* input_command, compiler_instructions_t* instructions);
label_instruction_return_e FixUp(compiler_instructions_t* instructions);

//=============== COMMAND_HANDLERS ==================

compiler_return_e ReadCommand(char** input_command, compiler_instructions_t* instructions);
compiler_return_e ReadPushArgument(char** input_command, compiler_instructions_t* instructions);
compiler_return_e ReadPopArgument(char** input_command, compiler_instructions_t* instructions);
compiler_return_e ReadJumpArgument(char** input_command, compiler_instructions_t* instructions);
compiler_return_e ReadCallArgument(char** input_command, compiler_instructions_t* instructions);

//=============== LABEL_TABULAR_USAGE ==================

label_tabular_t*           InitialiseLabelTabular();
label_instruction_return_e InitLabel(char* label_name, compiler_instructions_t* instructions);
label_instruction_return_e UseLabel(char* label_name, compiler_instructions_t* instructions);
void                       LabelTabularDump(compiler_instructions_t* instructions);
void                       DestroyLabelTabular(label_tabular_t* label_tabular);
bool                       CheckIfLabel(char* string);

struct compiler_command_t
{
    const char* command_name;
    uint8_t binary_value_block_1;
    uint8_t binary_value_block_2;
    enum compiler_return_e (*handler)(char** input_command, struct compiler_instructions_t* instructions);
};

const uint8_t EMPTY = 0b00000000;

const struct compiler_command_t COMPILER_COMMANDS_ARRAY[] = {
    {.command_name = "hlt",   .binary_value_block_1 = 0b00000000, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  0
    {.command_name = "push",  .binary_value_block_1 = 0b01000000, .binary_value_block_2 = EMPTY,      .handler = ReadPushArgument}, //USER_COMMANDS  1
    {.command_name = "out",   .binary_value_block_1 = 0b11000000, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  2
    {.command_name = "add",   .binary_value_block_1 = 0b11001000, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  3
    {.command_name = "sub",   .binary_value_block_1 = 0b11001001, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  4
    {.command_name = "mul",   .binary_value_block_1 = 0b11001010, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  5
    {.command_name = "div",   .binary_value_block_1 = 0b11001011, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  6
    {.command_name = "pop",   .binary_value_block_1 = 0b10000000, .binary_value_block_2 = EMPTY,      .handler = ReadPopArgument }, //USER_COMMANDS  7
    {.command_name = "sqrt",  .binary_value_block_1 = 0b11001100, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  8
    {.command_name = "in",    .binary_value_block_1 = 0b11000001, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  9
    {.command_name = "jmp",   .binary_value_block_1 = 0b11010000, .binary_value_block_2 = EMPTY,      .handler = ReadJumpArgument}, //USER_COMMANDS  10
    {.command_name = "ja",    .binary_value_block_1 = 0b11010001, .binary_value_block_2 = EMPTY,      .handler = ReadJumpArgument}, //USER_COMMANDS  11
    {.command_name = "jae",   .binary_value_block_1 = 0b11010010, .binary_value_block_2 = EMPTY,      .handler = ReadJumpArgument}, //USER_COMMANDS  12
    {.command_name = "jb",    .binary_value_block_1 = 0b11010011, .binary_value_block_2 = EMPTY,      .handler = ReadJumpArgument}, //USER_COMMANDS  13
    {.command_name = "jbe",   .binary_value_block_1 = 0b11010100, .binary_value_block_2 = EMPTY,      .handler = ReadJumpArgument}, //USER_COMMANDS  14
    {.command_name = "je",    .binary_value_block_1 = 0b11010101, .binary_value_block_2 = EMPTY,      .handler = ReadJumpArgument}, //USER_COMMANDS  15
    {.command_name = "jne",   .binary_value_block_1 = 0b11010111, .binary_value_block_2 = EMPTY,      .handler = ReadJumpArgument}, //USER_COMMANDS  16
    {.command_name = "call",  .binary_value_block_1 = 0b11011000, .binary_value_block_2 = EMPTY,      .handler = ReadCallArgument}, //USER_COMMANDS  17
    {.command_name = "ret",   .binary_value_block_1 = 0b11011001, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  18
    {.command_name =  NULL,   .binary_value_block_1 = 0b11100000, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  19
    {.command_name =  NULL,   .binary_value_block_1 = 0b11101000, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  20
    {.command_name =  NULL,   .binary_value_block_1 = 0b11110000, .binary_value_block_2 = EMPTY,      .handler = NULL            }, //USER_COMMANDS  21
    {.command_name = "drawb", .binary_value_block_1 = 0b11111000, .binary_value_block_2 = 0b00000000, .handler = NULL            }, //USER_COMMANDS  22
    {.command_name = "draw",  .binary_value_block_1 = 0b11111000, .binary_value_block_2 = 0b00000000, .handler = NULL            }};//USER_COMMANDS  23
const int COMMANDS_COUNT = sizeof(COMPILER_COMMANDS_ARRAY) / sizeof(COMPILER_COMMANDS_ARRAY[0]);

void   FreeAll(compiler_instructions_t* instructions, char* input_buffer);

#endif //COMPILER_COMMANDS_H
