#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>

enum  commands_e
{
    COMMAND_HLT = 0,
    COMMAND_INCORRECT_COMMAND = 1,
    COMMAND_VALID_SYNTAX = 2,
    COMMAND_INVALID_SYNTAX = 3,
    COMMAND_EMPTY_COMMAND = 4,
    COMMAND_PUSH = 5,
    COMMAND_OUT = 6,
    COMMAND_ADD = 7,
    COMMAND_SUB = 8,
    COMMAND_MUL = 9,
    COMMAND_DIV = 10};

struct command_t
{
    const char* command_name;
    const size_t number_of_arguments;
    const enum commands_e return_value;
};

const struct command_t COMMANDS_ARRAY[] = {
    {.command_name = "HLT",  .number_of_arguments = 0, .return_value = COMMAND_HLT,              },
    {.command_name = NULL,   .number_of_arguments = 0, .return_value = COMMAND_EMPTY_COMMAND,    },
    {.command_name = NULL,   .number_of_arguments = 0, .return_value = COMMAND_INCORRECT_COMMAND,},
    {.command_name = NULL,   .number_of_arguments = 0, .return_value = COMMAND_VALID_SYNTAX,     },
    {.command_name = NULL,   .number_of_arguments = 0, .return_value = COMMAND_INVALID_SYNTAX,   },
    {.command_name = "PUSH", .number_of_arguments = 1, .return_value = COMMAND_PUSH,             },
    {.command_name = "OUT",  .number_of_arguments = 0, .return_value = COMMAND_OUT,              },
    {.command_name = "ADD",  .number_of_arguments = 0, .return_value = COMMAND_ADD,              },
    {.command_name = "SUB",  .number_of_arguments = 0, .return_value = COMMAND_SUB,              },
    {.command_name = "MUL",  .number_of_arguments = 0, .return_value = COMMAND_MUL,              },
    {.command_name = "DIV",  .number_of_arguments = 0, .return_value = COMMAND_DIV,              }};
const size_t commands_count = sizeof(COMMANDS_ARRAY) / sizeof(COMMANDS_ARRAY[0]);

#endif //COMMANDS_H
