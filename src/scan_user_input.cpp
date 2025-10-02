#include "scan_user_input.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "Assert.h"
#include "color.h"
#include "stack.h"
#include "calculator.h"
#include "commands.h"

const size_t PROGRAM_INPUT_BUFFER_SIZE = 20;
static char command[PROGRAM_INPUT_BUFFER_SIZE] = {0};

void
ReadUserInput(stack_t* swag,
              calculator_commands_e*  input_command)
{
    ASSERT(swag != NULL);
    ASSERT(input_command != NULL);

    int character = 0;
    size_t count = 0;
    calculator_commands_e read_command = CALCULATOR_COMMAND_INCORRECT_COMMAND;

    memset(command, 0, PROGRAM_INPUT_BUFFER_SIZE);

    while ((character = getchar()) != '\n' && character != ' ' && count < PROGRAM_INPUT_BUFFER_SIZE)
    {
        command[count] = (char)character;
        count++;
    }
    ungetc(character, stdin);

    if (count == PROGRAM_INPUT_BUFFER_SIZE)
    {
        *input_command = CALCULATOR_COMMAND_BUFFER_OVERFLOW;
        return;
    }

    for (size_t index = 0; index < commands_count; index++)
    {
        if ((commands_array[index]).command_name == NULL)
        {
            continue;
        }
        if (strcmp(command, commands_array[index].command_name) == 0)
        {
            read_command = (commands_array[index]).return_value;
            break;
        }
    }

    *input_command = read_command;
}

void
ReadCoefficient(stack_t*               swag,
                calculator_commands_e* input_command)
{
    ASSERT(swag != NULL);
    ASSERT(input_command != NULL);

    value_type coefficient = 0;

    if (scanf("%d", &coefficient) != 1)
    {
        ClearBuffer();
        *input_command = CALCULATOR_COMMAND_INVALID_SYNTAX;
        return;
    }

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    stack_function_errors_e return_error = StackPush(swag, coefficient);
    if (return_error != STACK_FUNCTION_SUCCESS)
    {
        ClearBuffer();
        LOG_FUNCTION_ERROR(return_error);
        *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
        return;
    }

    *input_command = CALCULATOR_COMMAND_START;
}

bool ClearBuffer()
{
    bool flag = false;
    int character = 0;
    while ((character = getchar()) != '\n' && character != EOF)
    {
        if (!isspace(character))
        {
            flag = true;
        }
    }

    return flag;
}

void
CheckIfSpaces(calculator_commands_e* expected_state)
{
    if (ClearBuffer())
    {
        *expected_state = CALCULATOR_COMMAND_INVALID_SYNTAX;
    }
}

