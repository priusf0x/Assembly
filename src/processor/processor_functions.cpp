#include "processor_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Assert.h"
#include "color.h"
#include "logger.h"
#include "stack.h"
#include "tools.h"
#include "common_commands.h"

#define NDEBUG

const size_t START_STACK_SIZE = 8;

static processor_functions_return_value_e DoOperation(spu_t* spu,int (*operation)(int, int));

processor_functions_return_value_e
InitializeSPU(spu_t*      spu,
              const char* assembled_file_name)
{

    if (StackInit(&(spu->spu_stack), START_STACK_SIZE, "SPU stack") != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_INIT_STACK;
    }

    FILE* assembled_file = fopen(assembled_file_name, "rb");
    if (assembled_file == NULL)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_READ_INSTRUCTIONS;
    }

    int max_instruction_count = 0;
    fread(&(max_instruction_count) , sizeof(int), 1, assembled_file);

    spu->max_instruction_count = (size_t) max_instruction_count;
    spu->instructions = (int*) calloc((size_t) max_instruction_count, sizeof(int));
    if (spu->instructions == NULL)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR;
    }
    fread(spu->instructions, sizeof(int), (size_t)max_instruction_count, assembled_file);

    if (fclose(assembled_file) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_READ_INSTRUCTIONS;
    }

    spu->instruction_count = 0;

    spu->registers = (int*) calloc(PROCESSOR_REG_COUNT, sizeof(int));
    if (spu->registers == NULL)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR;
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
DestroySPU(spu_t* spu)
{
    StackDestroy(spu->spu_stack);
    free(spu->instructions);
    free(spu->registers);

    memset(spu, 0, sizeof(spu_t));

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
ExecuteInstructions(spu_t* spu)
{
    ASSERT(spu);
    int read_command = (spu->instructions)[spu->instruction_count];
    processor_functions_return_value_e processor_error = PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;

    while (read_command != COMMAND_HLT)
    {
        if (PROCESSOR_COMMANDS_ARRAY[read_command].command_function != NULL)
        {
            processor_error = PROCESSOR_COMMANDS_ARRAY[read_command].command_function (spu);
            if (processor_error != PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS)
            {
                printf(RED "ERROR NUMBER:...............................%d.\n"
                        RED "P" ORANGE "i" YELLOW "z" GREEN "d" BLUE "e" PURPLE "c " WHITE
                       "was happened...\n" STANDARD, processor_error);

                return processor_error;
            }
        }

        read_command = (spu->instructions)[spu->instruction_count];

        #ifndef NDEBUG
        ProcessorDump(spu);
        #endif
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

//================ COMMANDS_FUNCTIONS =========================ь

processor_functions_return_value_e
StackCommandIn(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value = 0;

    if (scanf("%d", &intermediate_value) != 1)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (StackPush(spu->spu_stack, intermediate_value) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackCommandPushFromReg(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;
    if (StackPush(spu->spu_stack, (spu->registers)[(spu->instructions)[spu->instruction_count]]) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackCommandPopToReg(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;
    if (StackPop(spu->spu_stack, &(spu->registers)[(spu->instructions)[spu->instruction_count]]) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackCommandPush(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;
    if (StackPush(spu->spu_stack, (spu->instructions)[spu->instruction_count]) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackOut(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value = 0;

    if (StackPop(spu->spu_stack, &intermediate_value) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    printf (GREEN "OUT-VALUE" WHITE " - %d\n" STANDARD, intermediate_value);

    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

// ================= COMPARATORS ==============================

static int comparator_above (int a, int b) {return (a > b);};
static int comparator_above_or_eq (int a, int b) {return (a >= b);};
static int comparator_below (int a, int b) {return (a < b);};
static int comparator_below_or_eq (int a, int b) {return (a <= b);};
static int comparator_eq (int a, int b) {return (a == b);};
static int comparator_not_eq (int a, int b) {return (a != b);};

// ================== JUMPS ======================

processor_functions_return_value_e
Jump(spu_t* spu)
{
    spu->instruction_count++;
    spu->instruction_count = (size_t) (spu->instructions)[spu->instruction_count];

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
JumpFunction(spu_t* spu,
             int (*comparator) (int, int))
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;


    if (StackPop(spu->spu_stack, &intermediate_value_1) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (StackPop(spu->spu_stack, &intermediate_value_2) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (comparator(intermediate_value_2, intermediate_value_1))
    {
        Jump(spu);
    }
    else
    {
        spu->instruction_count++;
        spu->instruction_count++;
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
JumpA(spu_t* spu)
{
    return JumpFunction(spu, comparator_above);
}

processor_functions_return_value_e
JumpAE(spu_t* spu)
{
    return JumpFunction(spu, comparator_above_or_eq);
}

processor_functions_return_value_e
JumpB(spu_t* spu)
{
    return JumpFunction(spu, comparator_below);
}

processor_functions_return_value_e
JumpBE(spu_t* spu)
{
    return JumpFunction(spu, comparator_below_or_eq);
}

processor_functions_return_value_e
JumpE(spu_t* spu)
{
    return JumpFunction(spu, comparator_eq);
}

processor_functions_return_value_e
JumpNE(spu_t* spu)
{
    return JumpFunction(spu, comparator_not_eq);
}

processor_functions_return_value_e
Call(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    if (StackPush(spu->spu_stack, (int) spu->instruction_count) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    spu->instruction_count++;
    spu->instruction_count = (size_t) (spu->instructions)[spu->instruction_count];

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
Return(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value = 0;

    if (StackPop(spu->spu_stack, &intermediate_value) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    spu->instruction_count = (size_t) intermediate_value + 2; //to skip function size

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}


// ================= ARITHMETIC OPERATIONS =======================

processor_functions_return_value_e
StackSqrt(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value = 0;

    if (StackPop(spu->spu_stack, &intermediate_value) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (intermediate_value < 0)
    {
        return PROCESSOR_FUNCTION_RETURN_SQRT_ERROR;
    }

    if (StackPush(spu->spu_stack, (int) sqrt(intermediate_value)) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static inline int sum_var(int intermediate_value_1, int intermediate_value_2) {return intermediate_value_2 + intermediate_value_1;}
static inline int sub_var(int intermediate_value_1, int intermediate_value_2) {return intermediate_value_2 - intermediate_value_1;}
static inline int mul_var(int intermediate_value_1, int intermediate_value_2) {return intermediate_value_2 * intermediate_value_1;}
static inline int div_var(int intermediate_value_1, int intermediate_value_2) {return intermediate_value_2 / intermediate_value_1;}

static processor_functions_return_value_e
DoOperation(spu_t* spu,
            int    (*operation)(int, int))
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    if (StackPop(spu->spu_stack, &intermediate_value_1) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (StackPop(spu->spu_stack, &intermediate_value_2) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if ((operation == div_var) && (intermediate_value_2 == 0))
    {
        return PROCESSOR_FUNCTION_RETURN_DIVISION_BY_ZERO;
    }

    if (StackPush(spu->spu_stack, operation(intermediate_value_1, intermediate_value_2)) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    PROCESSOR_VERIFY(spu);

    spu->instruction_count++;

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackAdd(spu_t* spu)
{
    return DoOperation(spu, sum_var);
}

processor_functions_return_value_e
StackSub(spu_t* spu)
{
    return DoOperation(spu, sub_var);
}

processor_functions_return_value_e
StackMul(spu_t* spu)
{
    return DoOperation(spu, mul_var);
}

processor_functions_return_value_e
StackDiv(spu_t* spu)
{
    return DoOperation(spu, div_var);
}

//================== VERIFICATION =====================

processor_functions_return_value_e
ProcessorDump(spu_t* spu)
{
    size_t index = 0;

    printf(YELLOW   "          ██▓███   ██▀███   ▒█████   ▄████▄  ▓█████   ██████   ██████  ▒█████    ██▀███  \n"
                    "         ▓██░  ██▒▓██ ▒ ██▒▒██▒  ██▒▒██▀ ▀█  ▓█   ▀ ▒██    ▒ ▒██    ▒ ▒██▒  ██▒▓██ ▒ ██▒ \n"
                    "         ▓██░ ██▓▒▓██ ░▄█ ▒▒██░  ██▒▒▓█    ▄ ▒███   ░ ▓██▄   ░ ▓██▄   ▒██░  ██▒▓██ ░▄█ ▒ \n"
                    "         ▒██▄█▓▒ ▒▒██▀▀█▄  ▒██   ██░▒▓▓▄ ▄██▒▒▓█  ▄   ▒   ██▒  ▒   ██▒▒██   ██░▒██▀▀█▄   \n"
                    "         ▒██▒ ░  ░░██▓ ▒██▒░ ████▓▒░▒ ▓███▀ ░░▒████▒▒██████▒▒▒██████▒▒░ ████▓▒░░██▓ ▒██▒ \n"
                    "         ▒▓▒░ ░  ░░ ▒▓ ░▒▓░░ ▒░▒░▒░ ░ ░▒ ▒  ░░░ ▒░ ░▒ ▒▓▒ ▒ ░▒ ▒▓▒ ▒ ░░ ▒░▒░▒░ ░ ▒▓ ░▒▓░ \n"
                    "         ░▒ ░       ░▒ ░ ▒░  ░ ▒ ▒░   ░  ▒    ░ ░  ░░ ░▒  ░ ░░ ░▒  ░ ░  ░ ▒ ▒░   ░▒ ░ ▒░ \n"
                    "         ░░         ░░   ░ ░ ░ ░ ▒  ░           ░   ░  ░  ░  ░  ░  ░  ░ ░ ░ ▒    ░░   ░  \n"
                    "                     ░         ░ ░  ░ ░         ░  ░      ░        ░      ░ ░     ░      \n"
                    "                                 ░                                                       \n");

    printf(YELLOW "__________________________________________________________________________________________________\n"
                  "-------------------------------------------COMMANDS-----------------------------------------------\n" STANDARD);
    do
    {
        if ((spu->instruction_count) == index)
        {
            printf(YELLOW "||" GREEN "%6d<---" STANDARD, (spu->instructions)[index]);
        }
        else
        {
            printf(YELLOW "||" WHITE "%6d    " STANDARD, (spu->instructions)[index]);
        }

        index++;

        if (index % 8 == 0)
        {
            printf(YELLOW "||\n" WHITE);
        }
    } while (index != spu->max_instruction_count);
    printf(YELLOW "||\n" WHITE);

    printf(YELLOW "              ___________________________________________________________________\n"
                  "              ----------------------------REGISTERS------------------------------\n              " STANDARD);

    for (size_t reg_index = 0; reg_index < PROCESSOR_REG_COUNT; reg_index++)
    {
        printf(YELLOW"||" RED "%s = " WHITE "%5d"  STANDARD, PROCESSORS_REG[reg_index], (spu->registers)[reg_index]);
    }

    printf(YELLOW "||\n");

    StackDump(spu->spu_stack);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

