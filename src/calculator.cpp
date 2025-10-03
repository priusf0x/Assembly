#include "calculator.h"

#include <stdio.h>

#include "Assert.h"
#include "color.h"
#include "commands.h"
#include "read_commands.h"
#include "logger.h"
#include "stack.h"


stack_function_errors_e StartCalculator(stack_t* calculator_stack)
{
    ASSERT(calculator_stack);

    VERIFY_STACK_WITH_RETURN(calculator_stack);

    calculator_commands_e input_command = CALCULATOR_COMMAND_START;

    while (input_command != CALCULATOR_COMMAND_HLT)
    {
        ((commands_array[input_command]).command_function)(calculator_stack, &input_command);
    }

    return STACK_FUNCTION_SUCCESS;
}

//================ COMMANDS_FUNCTIONS =========================

void
PrintError(stack_t*               calculator_stack,
           calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    printf(RED "Error was occupied.\n" STANDARD);
    *input_command = CALCULATOR_COMMAND_START;
}

void
PrintIncorrectSyntax(stack_t*               calculator_stack,
                    calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    printf(RED "Incorrect value.\n" STANDARD);
    *input_command = CALCULATOR_COMMAND_START;
}

void
CalculateStackOut(stack_t*               calculator_stack,
                  calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    value_type intermediate_value = 0;
    stack_function_errors_e return_error = StackPop(calculator_stack, &intermediate_value);

    if (return_error != STACK_FUNCTION_SUCCESS)
    {
        LOG_FUNCTION_ERROR(return_error);
        *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
        return;
    }
    printf("%d\n", intermediate_value);
    *input_command = CALCULATOR_COMMAND_START;
}

void
CalculateStackAdd(stack_t*               calculator_stack,
                  calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    stack_function_errors_e return_error = StackAdd(calculator_stack);
    if (return_error != STACK_FUNCTION_SUCCESS)
    {
        LOG_FUNCTION_ERROR(return_error);
        *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
        return;
    }

    *input_command = CALCULATOR_COMMAND_START;
}

void
CalculateStackSub(stack_t*               calculator_stack,
                  calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    stack_function_errors_e return_error = StackSub(calculator_stack);
    if (return_error != STACK_FUNCTION_SUCCESS)
    {
        LOG_FUNCTION_ERROR(return_error);
        *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
        return;
    }

    *input_command = CALCULATOR_COMMAND_START;
}

void
CalculateStackMul(stack_t*               calculator_stack,
                  calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    stack_function_errors_e return_error = StackMul(calculator_stack);
    if (return_error != STACK_FUNCTION_SUCCESS)
    {
        LOG_FUNCTION_ERROR(return_error);
        *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
        return;
    }

    *input_command = CALCULATOR_COMMAND_START;
}

void
CalculateStackDiv(stack_t*               calculator_stack,
                  calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    stack_function_errors_e return_error = StackDiv(calculator_stack);
    if (return_error != STACK_FUNCTION_SUCCESS)
    {
        LOG_FUNCTION_ERROR(return_error);
        *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
        return;
    }

    *input_command = CALCULATOR_COMMAND_START;
}

void
PrintIncorrectCommand(stack_t*               calculator_stack,
                      calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    ClearBuffer();
    printf(RED "Incorrect command.\n" STANDARD);
    *input_command = CALCULATOR_COMMAND_START;
}

void
PrintStack(stack_t*              calculator_stack,
          calculator_commands_e* input_command)
{
    ASSERT(calculator_stack != NULL);
    ASSERT(input_command != NULL);

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    for (size_t index = 0; index < calculator_stack->capacity; index++)
    {
        if (index < calculator_stack->size)
        {
            printf(GREEN "[%zu ELEMENT] %d \n" STANDARD, index + 1, (calculator_stack->stack_data)[index]);
        }
        else
        {
            printf(RED"[%zu ELEMENT] NOTHING_HERE \n" STANDARD, index + 1);
        }
    }

    *input_command = CALCULATOR_COMMAND_START;
}

