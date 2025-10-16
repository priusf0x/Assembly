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
    PROCESSOR_FUNCTION_RETURN_SCAN_FAILED,
    PROCESSOR_FUNCTION_RETURN_VERSIONS_MISMATCH
};

struct spu_t
{
    struct stack_t* spu_stack;
    int*            instructions;
    size_t          instruction_count;
    int*            registers;
    size_t          max_instruction_count;
    int*            RAM;
};

struct processor_command_t
{
    enum commands_e return_value;
    processor_functions_return_value_e (*command_function)(spu_t* spu);
};

processor_functions_return_value_e StackMul(spu_t* spu);
processor_functions_return_value_e StackAdd(spu_t* spu);
processor_functions_return_value_e StackSub(spu_t* spu);
processor_functions_return_value_e StackDiv(spu_t* spu);
processor_functions_return_value_e StackSqrt(spu_t* spu);
processor_functions_return_value_e Jump(spu_t* spu);
processor_functions_return_value_e JumpA(spu_t* spu);
processor_functions_return_value_e JumpAE(spu_t* spu);
processor_functions_return_value_e JumpB(spu_t* spu);
processor_functions_return_value_e JumpBE(spu_t* spu);
processor_functions_return_value_e JumpE(spu_t* spu);
processor_functions_return_value_e JumpNE(spu_t* spu);
processor_functions_return_value_e Call(spu_t* spu);
processor_functions_return_value_e Return(spu_t* spu);

processor_functions_return_value_e StackCommandPush(spu_t* spu);
processor_functions_return_value_e StackOut(spu_t* spu);
processor_functions_return_value_e StackCommandPopToMemory(spu_t* spu);
processor_functions_return_value_e StackCommandPopToReg(spu_t* spu);
processor_functions_return_value_e StackCommandPushFromMemory(spu_t* spu);
processor_functions_return_value_e StackCommandPushFromReg(spu_t* spu);
processor_functions_return_value_e StackCommandIn(spu_t* spu);
processor_functions_return_value_e DrawScreen(spu_t* spu);

processor_functions_return_value_e InitializeSPU(spu_t* spu, const char* assembled_file_name);
processor_functions_return_value_e ExecuteInstructions(spu_t* spu);
processor_functions_return_value_e DestroySPU(spu_t* spu);
processor_functions_return_value_e ProcessorDump(spu_t* spu);

const struct processor_command_t PROCESSOR_COMMANDS_ARRAY[] = {
    {.return_value = COMMAND_HLT,             .command_function = NULL                      },
    {.return_value = COMMAND_PUSH,            .command_function = StackCommandPush          },
    {.return_value = COMMAND_OUT,             .command_function = StackOut                  },
    {.return_value = COMMAND_ADD,             .command_function = StackAdd                  },
    {.return_value = COMMAND_SUB,             .command_function = StackSub                  },
    {.return_value = COMMAND_MUL,             .command_function = StackMul                  },
    {.return_value = COMMAND_DIV,             .command_function = StackDiv                  },
    {.return_value = COMMAND_PUSH_FROM_REG,   .command_function = StackCommandPushFromReg   },
    {.return_value = COMMAND_POP,             .command_function = StackCommandPopToReg      },
    {.return_value = COMMAND_SQRT,            .command_function = StackSqrt                 },
    {.return_value = COMMAND_IN,              .command_function = StackCommandIn            },
    {.return_value = COMMAND_JMP,             .command_function = Jump                      },
    {.return_value = COMMAND_JA,              .command_function = JumpA                     },
    {.return_value = COMMAND_JAE,             .command_function = JumpAE                    },
    {.return_value = COMMAND_JB,              .command_function = JumpB                     },
    {.return_value = COMMAND_JBE,             .command_function = JumpBE                    },
    {.return_value = COMMAND_JE,              .command_function = JumpE                     },
    {.return_value = COMMAND_JNE,             .command_function = JumpNE                    },
    {.return_value = COMMAND_CALL,            .command_function = Call                      },
    {.return_value = COMMAND_RET,             .command_function = Return                    },
    {.return_value = COMMAND_PUSH_FROM_MEMORY,.command_function = StackCommandPushFromMemory},
    {.return_value = COMMAND_POP_TO_MEMORY,   .command_function = StackCommandPopToMemory   },
    {.return_value = COMMAND_DRAW_B,          .command_function = DrawScreen                }};

const size_t PROCESSOR_COMMANDS_COUNT = sizeof(PROCESSOR_COMMANDS_ARRAY) / sizeof(PROCESSOR_COMMANDS_ARRAY[0]);

#ifdef NDEBUG
#define PROCESSOR_VERIFY(X)
#else
#define PROCESSOR_VERIFY(X)
#endif


#endif //PROCESSOR_FUNCTIONS_H
