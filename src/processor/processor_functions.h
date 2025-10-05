#ifndef PROCESSOR_FUNCTIONS_H
#define PROCESSOR_FUNCTIONS_H

#include <stdlib.h>

#include "logger.h"
#include "stack.h"

enum  processor_commands_e
{
    PROCESSOR_COMMAND_HLT  = 0,
    PROCESSOR_COMMAND_PUSH = 1,
    PROCESSOR_COMMAND_OUT  = 2,
    PROCESSOR_COMMAND_ADD  = 3,
    PROCESSOR_COMMAND_SUB  = 4,
    PROCESSOR_COMMAND_MUL  = 5,
    PROCESSOR_COMMAND_DIV  = 6,
};

struct spu_t
{
    struct stack_t* spu_stack;
    int*            instructions;
    size_t          instruction_count;
    int*            registers;
};

struct processor_command_t
{
    const char*               command_name = NULL;
    enum processor_commands_e return_value;
    void (*command_function)(spu_t* spu);
};


stack_function_errors_e StartCalculator(stack_t* calculator_stack);

void StackOut(spu_t* spu);
void StackAdd(spu_t* spu);
void StackSub(spu_t* spu);
void StackMul(spu_t* spu);
void StackDiv(spu_t* spu);

int InitializeSPU(spu_t* spu);

const struct processor_command_t PROCESSOR_COMMANDS_ARRAY[] = {
    {.command_name = "HLT",   .return_value = PROCESSOR_COMMAND_HLT,   .command_function = NULL},
    {.command_name = "PUSH",  .return_value = PROCESSOR_COMMAND_PUSH,  .command_function = NULL},
    {.command_name = "OUT",   .return_value = PROCESSOR_COMMAND_OUT,   .command_function = StackOut},
    {.command_name = "ADD",   .return_value = PROCESSOR_COMMAND_ADD,   .command_function = StackAdd},
    {.command_name = "SUB",   .return_value = PROCESSOR_COMMAND_SUB,   .command_function = StackSub},
    {.command_name = "MUL",   .return_value = PROCESSOR_COMMAND_MUL,   .command_function = StackMul},
    {.command_name = "DIV",   .return_value = PROCESSOR_COMMAND_DIV,   .command_function = StackDiv}};

const size_t PROCESSOR_COMMANDS_COUNT = sizeof(PROCESSOR_COMMANDS_ARRAY) / sizeof(PROCESSOR_COMMANDS_ARRAY[0]);

#endif //PROCESSOR_FUNCTIONS_H
