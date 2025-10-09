#ifndef COMPILER_COMMANDS_H
#define COMPILER_COMMANDS_H

#include <stdlib.h>

#include "common_commands.h"

struct compiler_instructions_t
{
    size_t instructions_count;
    size_t instructions_size;
    int* instructions_array;
};

enum  compiler_return_e
{
    COMPILER_RETURN_SUCCESS,
    COMPILER_RETURN_VALID_SYNTAX,
    COMPILER_RETURN_INCORRECT_COMMAND,
    COMPILER_RETURN_INVALID_SYNTAX,
    COMPILER_RETURN_EMPTY_COMMAND
};

compiler_return_e ReadPushArgument(char* input_command, compiler_instructions_t* instructions);
compiler_return_e ReadPopArgument(char* input_command, compiler_instructions_t* instructions);
compiler_return_e Readcommand(char* input_command, compiler_instructions_t* instructions);

struct compiler_command_t
{
    const char* command_name;
    const enum commands_e return_value;
    enum compiler_return_e (*handler)(char* input_command, struct compiler_instructions_t* instructions);
};

const struct compiler_command_t COMPILER_COMMANDS_ARRAY[] = {
    {.command_name = "HLT",   .return_value = COMMAND_HLT,              .handler = NULL            }, //USER_commandS  0
    {.command_name = "PUSH",  .return_value = COMMAND_PUSH,             .handler = ReadPushArgument}, //USER_commandS  1
    {.command_name = "OUT",   .return_value = COMMAND_OUT,              .handler = NULL            }, //USER_commandS  2
    {.command_name = "ADD",   .return_value = COMMAND_ADD,              .handler = NULL            }, //USER_commandS  3
    {.command_name = "SUB",   .return_value = COMMAND_SUB,              .handler = NULL            }, //USER_commandS  4
    {.command_name = "MUL",   .return_value = COMMAND_MUL,              .handler = NULL            }, //USER_commandS  5
    {.command_name = "DIV",   .return_value = COMMAND_DIV,              .handler = NULL            }, //USER_commandS  6
    {.command_name = NULL,    .return_value = COMMAND_PUSH_IN_REG,      .handler = NULL            }, //USER_commandS  7
    {.command_name = "POP",   .return_value = COMMAND_POP,              .handler = ReadPopArgument }, //USER_commandS  8
    {.command_name = "SQRT",  .return_value = COMMAND_SQRT,             .handler = NULL            }, //USER_commandS  9
    {.command_name = "IN",    .return_value = COMMAND_IN,               .handler = NULL            }}; //USER_commandS 10
const int COMMANDS_COUNT = sizeof(COMPILER_COMMANDS_ARRAY) / sizeof(COMPILER_COMMANDS_ARRAY[0]);

#endif //commandS_H
