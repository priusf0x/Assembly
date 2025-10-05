#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>

struct instructions_t
{
    size_t instructions_count;
    size_t instructions_size;
    int* instructions_array;
};

enum  commands_e
{
    COMMAND_HLT = 0,
    COMMAND_PUSH = 1,
    COMMAND_OUT = 2,
    COMMAND_ADD = 3,
    COMMAND_SUB = 4,
    COMMAND_MUL = 5,
    COMMAND_DIV = 6,
    COMMAND_INCORRECT_COMMAND = 7,
    COMMAND_VALID_SYNTAX = 8,
    COMMAND_INVALID_SYNTAX = 9,
    COMMAND_EMPTY_COMMAND = 10,
};

commands_e ReadPushArgument(char* input_command, instructions_t* instructions);
commands_e ReadCommand(char* input_command, instructions_t* instructions);

struct command_t
{
    const char* command_name;
    const enum commands_e return_value;
    enum commands_e (*handler)(char* input_command, struct instructions_t* instructions);
};

const struct command_t COMMANDS_ARRAY[] = {
    {.command_name = "HLT",   .return_value = COMMAND_HLT,              .handler = NULL},
    {.command_name = "PUSH",  .return_value = COMMAND_PUSH,             .handler = ReadPushArgument},
    {.command_name = "OUT",   .return_value = COMMAND_OUT,              .handler = NULL},
    {.command_name = "ADD",   .return_value = COMMAND_ADD,              .handler = NULL},
    {.command_name = "SUB",   .return_value = COMMAND_SUB,              .handler = NULL},
    {.command_name = "MUL",   .return_value = COMMAND_MUL,              .handler = NULL},
    {.command_name = "DIV",   .return_value = COMMAND_DIV,              .handler = NULL},
    {.command_name = NULL,    .return_value = COMMAND_EMPTY_COMMAND,    .handler = NULL},
    {.command_name = NULL,    .return_value = COMMAND_INCORRECT_COMMAND,.handler = NULL},
    {.command_name = NULL,    .return_value = COMMAND_VALID_SYNTAX,     .handler = NULL},
    {.command_name = NULL,    .return_value = COMMAND_INVALID_SYNTAX,   .handler = NULL}};

const size_t COMMANDS_COUNT = sizeof(COMMANDS_ARRAY) / sizeof(COMMANDS_ARRAY[0]);


#endif //COMMANDS_H
