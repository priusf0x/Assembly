#ifndef PROCESSOR_FUNCTIONS_H
#define PROCESSOR_FUNCTIONS_H

#include <stdlib.h>

#include "logger.h"
#include "stack.h"

enum processor_functions_return_value_e
{
    PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS,
    PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_READ_INSTRUCTIONS,
    PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_INIT_STACK,
    PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR,
    PROCESSOR_FUNCTION_RETURN_STACK_ERROR
};

enum  processor_commands_e
{
    PROCESSOR_COMMAND_HLT          = 0,
    PROCESSOR_COMMAND_PUSH         = 1,
    PROCESSOR_COMMAND_OUT          = 2,
    PROCESSOR_COMMAND_ADD          = 3,
    PROCESSOR_COMMAND_SUB          = 4,
    PROCESSOR_COMMAND_MUL          = 5,
    PROCESSOR_COMMAND_DIV          = 6,
    PROCESSOR_COMMAND_PUSH_IN_REG  = 7,
    PROCESSOR_COMMAND_POP          = 8
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
    enum processor_commands_e return_value;
    processor_functions_return_value_e (*command_function)(spu_t* spu);
};

processor_functions_return_value_e StackCommandPush(spu_t* spu);
processor_functions_return_value_e StackOut(spu_t* spu);
processor_functions_return_value_e StackMul(spu_t* spu);
processor_functions_return_value_e StackAdd(spu_t* spu);
processor_functions_return_value_e StackSub(spu_t* spu);
processor_functions_return_value_e StackDiv(spu_t* spu);

processor_functions_return_value_e InitializeSPU(spu_t* spu);
processor_functions_return_value_e ExecuteInstructions(spu_t* spu);
processor_functions_return_value_e DestroySPU(spu_t* spu);

const struct processor_command_t PROCESSOR_COMMANDS_ARRAY[] = {
    {.return_value = PROCESSOR_COMMAND_HLT,   .command_function = NULL            },
    {.return_value = PROCESSOR_COMMAND_PUSH,  .command_function = StackCommandPush},
    {.return_value = PROCESSOR_COMMAND_OUT,   .command_function = StackOut        },
    {.return_value = PROCESSOR_COMMAND_ADD,   .command_function = StackAdd        },
    {.return_value = PROCESSOR_COMMAND_SUB,   .command_function = StackSub        },
    {.return_value = PROCESSOR_COMMAND_MUL,   .command_function = StackMul        },
    {.return_value = PROCESSOR_COMMAND_DIV,   .command_function = StackDiv        }};

const size_t PROCESSOR_COMMANDS_COUNT = sizeof(PROCESSOR_COMMANDS_ARRAY) / sizeof(PROCESSOR_COMMANDS_ARRAY[0]);

#ifdef NVERIFY
#define PROCESSOR_VERIFY(X)
#else
#define PROCESSOR_VERIFY(X)
#endif


#endif //PROCESSOR_FUNCTIONS_H
