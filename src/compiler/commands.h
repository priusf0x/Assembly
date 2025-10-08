#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>

struct instructions_t
{
    size_t instructions_count;
    size_t instructions_size;
    int* instructions_array;
};

enum  compiler_commands_e
{
    COMPILER_COMMAND_HLT               = 0,
    COMPILER_COMMAND_PUSH              = 1,
    COMPILER_COMMAND_OUT               = 2,
    COMPILER_COMMAND_ADD               = 3,
    COMPILER_COMMAND_SUB               = 4,
    COMPILER_COMMAND_MUL               = 5,
    COMPILER_COMMAND_DIV               = 6,
    COMPILER_COMMAND_PUSH_IN_REG       = 7,
    COMPILER_COMMAND_POP               = 8,
    COMPILER_COMMAND_SQRT              = 9,
    COMPILER_COMMAND_IN                = 10,
    COMPILER_COMMAND_INCORRECT_COMMAND = 11,
    COMPILER_COMMAND_VALID_SYNTAX      = 12,
    COMPILER_COMMAND_INVALID_SYNTAX    = 13,
    COMPILER_COMMAND_EMPTY_COMMAND     = 14
};

compiler_commands_e ReadPushArgument(char* input_command, instructions_t* instructions);
compiler_commands_e ReadPopArgument(char* input_command, instructions_t* instructions);
compiler_commands_e ReadCommand(char* input_command, instructions_t* instructions);

struct command_t
{
    const char* command_name;
    const enum compiler_commands_e return_value;
    enum compiler_commands_e (*handler)(char* input_command, struct instructions_t* instructions);
};

const struct command_t COMMANDS_ARRAY[] = {
    {.command_name = "HLT",   .return_value = COMPILER_COMMAND_HLT,              .handler = NULL            }, //USER_COMMANDS  0
    {.command_name = "PUSH",  .return_value = COMPILER_COMMAND_PUSH,             .handler = ReadPushArgument}, //USER_COMMANDS  1
    {.command_name = "OUT",   .return_value = COMPILER_COMMAND_OUT,              .handler = NULL            }, //USER_COMMANDS  2
    {.command_name = "ADD",   .return_value = COMPILER_COMMAND_ADD,              .handler = NULL            }, //USER_COMMANDS  3
    {.command_name = "SUB",   .return_value = COMPILER_COMMAND_SUB,              .handler = NULL            }, //USER_COMMANDS  4
    {.command_name = "MUL",   .return_value = COMPILER_COMMAND_MUL,              .handler = NULL            }, //USER_COMMANDS  5
    {.command_name = "DIV",   .return_value = COMPILER_COMMAND_DIV,              .handler = NULL            }, //USER_COMMANDS  6
    {.command_name = NULL,    .return_value = COMPILER_COMMAND_PUSH_IN_REG,      .handler = NULL            }, //USER_COMMANDS  7
    {.command_name = "POP",   .return_value = COMPILER_COMMAND_POP,              .handler = ReadPopArgument }, //USER_COMMANDS  8
    {.command_name = "SQRT",  .return_value = COMPILER_COMMAND_SQRT,             .handler = NULL            }, //USER_COMMANDS  9
    {.command_name = "IN",    .return_value = COMPILER_COMMAND_IN,               .handler = NULL            }, //USER_COMMANDS 10
    {.command_name = NULL,    .return_value = COMPILER_COMMAND_EMPTY_COMMAND,    .handler = NULL            },
    {.command_name = NULL,    .return_value = COMPILER_COMMAND_INCORRECT_COMMAND,.handler = NULL            },
    {.command_name = NULL,    .return_value = COMPILER_COMMAND_VALID_SYNTAX,     .handler = NULL            },
    {.command_name = NULL,    .return_value = COMPILER_COMMAND_INVALID_SYNTAX,   .handler = NULL            }};
const int COMMANDS_COUNT = sizeof(COMMANDS_ARRAY) / sizeof(COMMANDS_ARRAY[0]);

const char* const PROCESSORS_REG[] =
{
    "R0X",
    "RAX",
    "RBX",
    "RCX",
    "RDX"
};
const int PROCESSOR_REG_COUNT = sizeof(PROCESSORS_REG) /  sizeof(PROCESSORS_REG[0]);

#endif //COMMANDS_H
