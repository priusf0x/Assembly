#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>

#include "logger.h"
#include "stack.h"
#include "calculator.h"
#include "read_commands.h"

static const struct command_t commands_array[] = {
    {.command_name = NULL,    .return_value = CALCULATOR_COMMAND_INCORRECT_COMMAND, .command_function = PrintIncorrectCommand},
    {.command_name = NULL,    .return_value = CALCULATOR_COMMAND_PROGRAM_ERROR,     .command_function = PrintError},
    {.command_name = NULL,    .return_value = CALCULATOR_COMMAND_INVALID_SYNTAX,    .command_function = PrintIncorrectSyntax},
    {.command_name = "HLT",   .return_value = CALCULATOR_COMMAND_HLT,               .command_function = NULL},
    {.command_name = "PUSH",  .return_value = CALCULATOR_COMMAND_PUSH,              .command_function = ReadCoefficient},
    {.command_name = "OUT",   .return_value = CALCULATOR_COMMAND_OUT,               .command_function = CalculateStackOut},
    {.command_name = "PRINT", .return_value = CALCULATOR_COMMAND_PRINT,             .command_function = PrintStack},
    {.command_name = "ADD",   .return_value = CALCULATOR_COMMAND_ADD,               .command_function = CalculateStackAdd},
    {.command_name = "SUB",   .return_value = CALCULATOR_COMMAND_SUB,               .command_function = CalculateStackSub},
    {.command_name = "MUL",   .return_value = CALCULATOR_COMMAND_MUL,               .command_function = CalculateStackMul},
    {.command_name = "DIV",   .return_value = CALCULATOR_COMMAND_DIV,               .command_function = CalculateStackDiv}};

const size_t commands_count = sizeof(commands_array) / sizeof(commands_array[0]);

#endif //COMMANDS_H
