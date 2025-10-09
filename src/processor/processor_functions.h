#ifndef PROCESSOR_FUNCTIONS_H
#define PROCESSOR_FUNCTIONS_H

#include <stdlib.h>

#include "common_commands.h"
#include "logger.h"
#include "stack.h"

enum processor_functions_return_value_e
{
    PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS,
    PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_READ_INSTRUCTIONS,
    PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_INIT_STACK,
    PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR,
    PROCESSOR_FUNCTION_RETURN_STACK_ERROR,
    PROCESSOR_FUNCTION_RETURN_DIVISION_BY_ZERO,
    PROCESSOR_FUNCTION_RETURN_SQRT_ERROR,
    PROCESSOR_FUNCTION_RETURN_SCAN_FAILED
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
    enum commands_e return_value;
    processor_functions_return_value_e (*command_function)(spu_t* spu);
};

processor_functions_return_value_e StackcommandPush(spu_t* spu);
processor_functions_return_value_e StackOut(spu_t* spu);
processor_functions_return_value_e StackMul(spu_t* spu);
processor_functions_return_value_e StackAdd(spu_t* spu);
processor_functions_return_value_e StackSub(spu_t* spu);
processor_functions_return_value_e StackDiv(spu_t* spu);
processor_functions_return_value_e StackSqrt(spu_t* spu);
processor_functions_return_value_e StackcommandPopToReg(spu_t* spu);
processor_functions_return_value_e StackcommandPushFromReg(spu_t* spu);
processor_functions_return_value_e StackcommandIn(spu_t* spu);

processor_functions_return_value_e InitializeSPU(spu_t* spu);
processor_functions_return_value_e ExecuteInstructions(spu_t* spu);
processor_functions_return_value_e DestroySPU(spu_t* spu);
processor_functions_return_value_e ProcessorDump(spu_t* spu);

const struct processor_command_t PROCESSOR_COMMANDS_ARRAY[] = {
    {.return_value = COMMAND_HLT,           .command_function = NULL                   },
    {.return_value = COMMAND_PUSH,          .command_function = StackcommandPush       },
    {.return_value = COMMAND_OUT,           .command_function = StackOut               },
    {.return_value = COMMAND_ADD,           .command_function = StackAdd               },
    {.return_value = COMMAND_SUB,           .command_function = StackSub               },
    {.return_value = COMMAND_MUL,           .command_function = StackMul               },
    {.return_value = COMMAND_DIV,           .command_function = StackDiv               },
    {.return_value = COMMAND_PUSH_IN_REG,   .command_function = StackcommandPushFromReg},
    {.return_value = COMMAND_POP,           .command_function = StackcommandPopToReg   },
    {.return_value = COMMAND_SQRT,          .command_function = StackSqrt              },
    {.return_value = COMMAND_IN,            .command_function = StackcommandIn         }};

const size_t PROCESSOR_COMMANDS_COUNT = sizeof(PROCESSOR_COMMANDS_ARRAY) / sizeof(PROCESSOR_COMMANDS_ARRAY[0]);

#ifdef NVERIFY
#define PROCESSOR_VERIFY(X)
#else
#define PROCESSOR_VERIFY(X)
#endif


#endif //PROCESSOR_FUNCTIONS_H
