#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stdlib.h>

#include "logger.h"
#include "stack.h"

enum  calculator_commands_e
{
    CALCULATOR_COMMAND_START = 0,
    CALCULATOR_COMMAND_INCORRECT_COMMAND = 1,
    CALCULATOR_COMMAND_PROGRAM_ERROR = 2,
    CALCULATOR_COMMAND_INVALID_SYNTAX = 3,
    CALCULATOR_COMMAND_HLT = 4,
    CALCULATOR_COMMAND_PUSH = 5,
    CALCULATOR_COMMAND_OUT = 6,
    CALCULATOR_COMMAND_PRINT = 7,
    CALCULATOR_COMMAND_ADD = 8,
    CALCULATOR_COMMAND_SUB = 9,
    CALCULATOR_COMMAND_MUL = 10,
    CALCULATOR_COMMAND_DIV = 11
};

struct command_t
{
    const char* command_name = NULL;
    enum calculator_commands_e return_value;
    void (*command_function)(struct stack_t*, enum calculator_commands_e*);
};

void                    PrintIncorrectSyntax(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    PrintError(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    CalculateStackOut(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    CalculateStackAdd(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    CalculateStackSub(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    CalculateStackMul(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    CalculateStackDiv(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    PrintBufferOverflow(stack_t* calculator_stack, calculator_commands_e* input_command);
void                    PrintStack(struct stack_t* calculator_stack, calculator_commands_e* input_command);
void                    PrintIncorrectCommand(stack_t* calculator_stack, calculator_commands_e* input_command);
stack_function_errors_e StartCalculator(stack_t* calculator_stack);


#ifdef SKIP_STACK_VERIFICATION
#define LOG_FUNCTION_ERROR(X) X
#else
#define LOG_FUNCTION_ERROR(X) if (X != STACK_FUNCTION_SUCCESS) {LOGSHIT(DETALIZATION_LEVEL_ERROR, "STACK_FUNCTION_ERROR %d was occupied in line %d in %s.", X, __LINE__, __FILE__);}
#endif

#endif //CALCULATOR_H
