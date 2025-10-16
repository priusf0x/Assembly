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
    size_t           instructions_count;
    size_t           instructions_size;
    int*             instructions_array;
    label_tabular_t* instructions_label_tabular;
};

enum  compiler_return_e
{
    COMPILER_RETURN_SUCCESS,
    COMPILER_RETURN_VALID_SYNTAX,
    COMPILER_RETURN_INCORRECT_COMMAND,
    COMPILER_RETURN_INVALID_SYNTAX,
    COMPILER_RETURN_EMPTY_COMMAND,
    COMPILER_RETURN_LABEL_ERROR
};

compiler_return_e ReadPushArgument(char** input_command, compiler_instructions_t* instructions);
compiler_return_e ReadPopArgument(char** input_command, compiler_instructions_t* instructions);
compiler_return_e ReadJumpArgument(char** input_command, compiler_instructions_t* instructions);
compiler_return_e TranslateCode(char* input_command, compiler_instructions_t* instructions);
compiler_return_e ReadCommand(char** input_command, compiler_instructions_t* instructions);
compiler_return_e ReadCallArgument(char** input_command, compiler_instructions_t* instructions);

bool                       CheckIfLabel(char* string);
label_tabular_t*           InitialiseLabelTabular();
label_instruction_return_e InitLabel(char* label_name, compiler_instructions_t* instructions);
label_instruction_return_e UseLabel(char* label_name, compiler_instructions_t* instructions);
label_instruction_return_e FixUp(compiler_instructions_t* instructions);
void                       LabelTabularDump(compiler_instructions_t* instructions);
void                       DestroyLabelTabular(label_tabular_t* label_tabular);

struct compiler_command_t
{
    const char* command_name;
    const enum commands_e return_value;
    enum compiler_return_e (*handler)(char** input_command, struct compiler_instructions_t* instructions);
};

const struct compiler_command_t COMPILER_COMMANDS_ARRAY[] = {
    {.command_name = "hlt",   .return_value = COMMAND_HLT,              .handler = NULL            }, //USER_COMMANDS  0
    {.command_name = "push",  .return_value = COMMAND_PUSH,             .handler = ReadPushArgument}, //USER_COMMANDS  1
    {.command_name = "out",   .return_value = COMMAND_OUT,              .handler = NULL            }, //USER_COMMANDS  2
    {.command_name = "add",   .return_value = COMMAND_ADD,              .handler = NULL            }, //USER_COMMANDS  3
    {.command_name = "sub",   .return_value = COMMAND_SUB,              .handler = NULL            }, //USER_COMMANDS  4
    {.command_name = "mul",   .return_value = COMMAND_MUL,              .handler = NULL            }, //USER_COMMANDS  5
    {.command_name = "div",   .return_value = COMMAND_DIV,              .handler = NULL            }, //USER_COMMANDS  6
    {.command_name = NULL,    .return_value = COMMAND_PUSH_FROM_REG,    .handler = NULL            }, //USER_COMMANDS  7
    {.command_name = "pop",   .return_value = COMMAND_POP,              .handler = ReadPopArgument }, //USER_COMMANDS  8
    {.command_name = "sqrt",  .return_value = COMMAND_SQRT,             .handler = NULL            }, //USER_COMMANDS  9
    {.command_name = "in",    .return_value = COMMAND_IN,               .handler = NULL            }, //USER_COMMANDS  10
    {.command_name = "jmp",   .return_value = COMMAND_JMP,              .handler = ReadJumpArgument}, //USER_COMMANDS  11
    {.command_name = "ja",    .return_value = COMMAND_JA,               .handler = ReadJumpArgument}, //USER_COMMANDS  12
    {.command_name = "jae",   .return_value = COMMAND_JAE,              .handler = ReadJumpArgument}, //USER_COMMANDS  13
    {.command_name = "jb",    .return_value = COMMAND_JB,               .handler = ReadJumpArgument}, //USER_COMMANDS  14
    {.command_name = "jbe",   .return_value = COMMAND_JBE,              .handler = ReadJumpArgument}, //USER_COMMANDS  15
    {.command_name = "je",    .return_value = COMMAND_JE,               .handler = ReadJumpArgument}, //USER_COMMANDS  16
    {.command_name = "jne",   .return_value = COMMAND_JNE,              .handler = ReadJumpArgument}, //USER_COMMANDS  17
    {.command_name = "call",  .return_value = COMMAND_CALL,             .handler = ReadCallArgument}, //USER_COMMANDS  18
    {.command_name = "ret",   .return_value = COMMAND_RET,              .handler = NULL            }, //USER_COMMANDS  19
    {.command_name = NULL,    .return_value = COMMAND_PUSH_FROM_MEMORY, .handler = ReadCallArgument}, //USER_COMMANDS  20
    {.command_name = NULL,    .return_value = COMMAND_POP_TO_MEMORY,    .handler = NULL            }, //USER_COMMANDS  21
    {.command_name = "drawb", .return_value = COMMAND_DRAW_B,           .handler = NULL            }}; //USER_COMMANDS 22
const int COMMANDS_COUNT = sizeof(COMPILER_COMMANDS_ARRAY) / sizeof(COMPILER_COMMANDS_ARRAY[0]);

void   FreeAll(compiler_instructions_t* instructions, char* input_buffer);

#endif //COMPILER_COMMANDS_H
