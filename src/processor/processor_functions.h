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
    PROCESSOR_FUNCTION_RETURN_PROCESSOR_MEMORY_SANITIZER,
    PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR,
    PROCESSOR_FUNCTION_RETURN_STACK_ERROR,
    PROCESSOR_FUNCTION_RETURN_INCORRECT_COMMAND,
    PROCESSOR_FUNCTION_RETURN_DIVISION_BY_ZERO,
    PROCESSOR_FUNCTION_RETURN_SQRT_ERROR,
    PROCESSOR_FUNCTION_RETURN_SCAN_FAILED,
    PROCESSOR_FUNCTION_RETURN_VERSIONS_MISMATCH,
    PROCESSOR_FUNCTION_RETURN_OUT_OF_MEMORY
};

struct spu_t
{
    struct stack_t* spu_stack;
    uint8_t*        instructions;
    size_t          read_bytes_amount;
    int*            registers;
    size_t          max_bytes_amount;
    int*            RAM;
};

struct processor_command_t
{
    enum commands_e return_value;
    processor_functions_return_value_e (*command_function)(spu_t* spu);
};

processor_functions_return_value_e StackCommandPush(spu_t* spu);
processor_functions_return_value_e StackCommandPop(spu_t* spu);
processor_functions_return_value_e StackInOut(spu_t* spu);
processor_functions_return_value_e StackDoOperation(spu_t* spu);
processor_functions_return_value_e JumpFunction(spu_t* spu);
processor_functions_return_value_e Return(spu_t* spu);
processor_functions_return_value_e Call(spu_t* spu);
processor_functions_return_value_e DrawScreen(spu_t* spu);
processor_functions_return_value_e DrawScreenQuadro(spu_t* spu);

processor_functions_return_value_e InitializeSPU(spu_t* spu, const char* assembled_file_name);
processor_functions_return_value_e ExecuteInstructions(spu_t* spu);
processor_functions_return_value_e DestroySPU(spu_t* spu);
processor_functions_return_value_e ProcessorDump(spu_t* spu);

const struct processor_command_t PROCESSOR_COMMANDS_ARRAY[] =
{
    {.return_value = COMMAND_HLT,             .command_function = NULL            },
    {.return_value = COMMAND_PUSH,            .command_function = StackCommandPush},
    {.return_value = COMMAND_POP,             .command_function = StackCommandPop },
    {.return_value = COMMAND_OUT,             .command_function = StackInOut      },
    {.return_value = COMMAND_OPERATION,       .command_function = StackDoOperation},
    {.return_value = COMMAND_JMP,             .command_function = JumpFunction    },
    {.return_value = COMMAND_CALL,            .command_function = Call            },
    {.return_value = COMMAND_DRAW,            .command_function = DrawScreen      }
};

const size_t PROCESSOR_COMMANDS_COUNT = sizeof(PROCESSOR_COMMANDS_ARRAY) / sizeof(PROCESSOR_COMMANDS_ARRAY[0]);

#ifdef NDEBUG
#define PROCESSOR_VERIFY(X)
#else
#define PROCESSOR_VERIFY(X)
#endif


#endif //PROCESSOR_FUNCTIONS_H
