#include "processor_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

#include "Assert.h"
#include "color.h"
#include "logger.h"
#include "stack.h"
#include "tools.h"
#include "common_commands.h"

#define NDEBUG
#define VIDEO_PLAY
// #define SHOW_RAM

const size_t START_STACK_SIZE = 8;
const uint64_t PROCESSOR_VERSION = 4;
const size_t SCREEN_SIZE_X = 98;
const size_t SCREEN_SIZE_Y = 36;
const size_t RAM_SIZE = 4000;

static void PrintRAMData(spu_t* spu);

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

    uint64_t compiler_version = 0;
    fread(&compiler_version , sizeof(uint64_t), 1, assembled_file);
    if (compiler_version != PROCESSOR_VERSION)
    {
        printf(RED "VERSION OF COMPILER IS MISMATCHING WITH VERSION OF DISASSEMBLER.\n" STANDARD);
        return PROCESSOR_FUNCTION_RETURN_VERSIONS_MISMATCH;
    }

    size_t max_bytes_amount = 0;
    fread(&(max_bytes_amount) , sizeof(size_t), 1, assembled_file);

    spu->max_bytes_amount = max_bytes_amount;

    spu->instructions = (uint8_t*) calloc(max_bytes_amount, sizeof(uint8_t));
    if (spu->instructions == NULL)
    {
        fclose(assembled_file);
        return PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR;
    }

    fread(spu->instructions, sizeof(uint8_t), (size_t) max_bytes_amount, assembled_file);

    if (fclose(assembled_file) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_READ_INSTRUCTIONS;
    }

    spu->read_bytes_amount = 0;

    spu->registers = (int*) calloc(PROCESSOR_REG_COUNT, sizeof(int));
    if (spu->registers == NULL)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR;
    }

    spu->RAM = (int*) calloc(RAM_SIZE, sizeof(int));
    if (spu->RAM == NULL)
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
    free(spu->RAM);

    memset(spu, 0, sizeof(spu_t));

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
ExecuteInstructions(spu_t* spu)
{
    ASSERT(spu);

    size_t command_index = TranslateCommandNumber(spu->instructions, &(spu->read_bytes_amount));
    processor_functions_return_value_e processor_error = PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;

    while (command_index)
    {
        if (PROCESSOR_COMMANDS_ARRAY[command_index].command_function != NULL)
        {
            processor_error = PROCESSOR_COMMANDS_ARRAY[command_index].command_function (spu);
            if (processor_error != PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS)
            {
                ProcessorDump(spu);
                printf(RED "ERROR NUMBER:...............................%d.\n"
                        RED "P" ORANGE "i" YELLOW "z" GREEN "d" BLUE "e" PURPLE "c " WHITE
                       "was happened...\n" STANDARD, processor_error);

                return processor_error;
            }
        }
        // ProcessorDump(spu);

        command_index = TranslateCommandNumber(spu->instructions, &(spu->read_bytes_amount));
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

//================ COMMANDS_FUNCTIONS =========================ь

processor_functions_return_value_e
DrawScreen(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    fprintf(stdout, "\e[1;1H\e[2J"); //clear screen

    if (spu->instructions[spu->read_bytes_amount] & ARGUMENT_MASK)
    {
        fwrite((uint8_t*) spu->RAM, sizeof(uint8_t), SCREEN_SIZE_X * SCREEN_SIZE_Y, stdout);
    }
    else
    {
        fwrite(spu->RAM, sizeof(int), SCREEN_SIZE_X * SCREEN_SIZE_Y, stdout);
    }

    #ifdef VIDEO_PLAY
        usleep( 20000 );
    #endif

    PROCESSOR_VERIFY(spu);

    spu->read_bytes_amount += sizeof(uint8_t);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackCommandPush(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    size_t*  command_index = &(spu->read_bytes_amount);
    uint8_t* instructions = spu->instructions;

    if ((instructions[*command_index] & USES_RAM) && (instructions[*command_index] & USES_INT))
    {
        return PROCESSOR_FUNCTION_RETURN_INCORRECT_COMMAND;
    }
    else if (instructions[*command_index] & USES_INT)
    {
        *command_index += sizeof(uint8_t);
        if (StackPush(spu->spu_stack, *((int*) (spu->instructions + *command_index))) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
        *command_index += sizeof(int);
    }
    else if (instructions[*command_index] & USES_RAM)
    {
        uint8_t spu_reg = (spu->instructions)[*command_index] & REGISTER_MASK;
        if (StackPush(spu->spu_stack, (spu->RAM)[(spu->registers)[spu_reg]]) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
        *command_index += sizeof(uint8_t);
    }
    else
    {
        uint8_t spu_reg = (spu->instructions)[*command_index] & REGISTER_MASK;
        if (StackPush(spu->spu_stack, (spu->registers)[spu_reg]) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
        *command_index += sizeof(uint8_t);
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackCommandPop(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    size_t*  command_index = &(spu->read_bytes_amount);
    uint8_t* instructions = spu->instructions;

    if ((instructions[*command_index] & USES_RAM) && (instructions[*command_index] & USES_INT))
    {
        return PROCESSOR_FUNCTION_RETURN_INCORRECT_COMMAND;
    }
    else if (instructions[*command_index] & USES_RAM)
    {
        uint8_t spu_reg = (spu->instructions)[*command_index] & REGISTER_MASK;
        if (StackPop(spu->spu_stack, &((spu->RAM)[spu->registers[spu_reg]])) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
        *command_index += sizeof(uint8_t);
    }
    else
    {
        uint8_t spu_reg = (spu->instructions)[*command_index] & REGISTER_MASK;
        if (StackPop(spu->spu_stack, &spu->registers[spu_reg]) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
        *command_index += sizeof(uint8_t);
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackInOut(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value = 0;

    if (spu->instructions[spu->read_bytes_amount] & ARGUMENT_MASK)
    {
        if (scanf("%d", &intermediate_value) != 1)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }

        if (StackPush(spu->spu_stack, intermediate_value) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
    }
    else
    {
        if (StackPop(spu->spu_stack, &intermediate_value) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
        printf(GREEN "OUT-VALUE" WHITE " - %d\n" STANDARD, intermediate_value);
    }

    PROCESSOR_VERIFY(spu);

    spu->read_bytes_amount += sizeof(uint8_t);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

// ================= COMPARATORS ==============================

static int comparator_empty (int , int) {return 1;};
static int comparator_above (int a, int b) {return (a > b);};
static int comparator_above_or_eq (int a, int b) {return (a >= b);};
static int comparator_below (int a, int b) {return (a < b);};
static int comparator_below_or_eq (int a, int b) {return (a <= b);};
static int comparator_eq (int a, int b) {return (a == b);};
static int comparator_not_eq (int a, int b) {return (a != b);};

// ================== JUMPS ======================

static processor_functions_return_value_e
Jump(spu_t* spu)
{
    spu->read_bytes_amount += sizeof(uint8_t);
    spu->read_bytes_amount = (size_t) *(int*) (spu->instructions + spu->read_bytes_amount);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
JumpFunction(spu_t* spu)
{
    int (*comparators[]) (int a, int b)
    {
        comparator_empty,
        comparator_above,
        comparator_above_or_eq,
        comparator_below,
        comparator_below_or_eq,
        comparator_eq,
        comparator_not_eq
    };

    PROCESSOR_VERIFY(spu);

    if (!(spu->instructions[spu->read_bytes_amount] & ARGUMENT_MASK))
    {
        Jump(spu);
        return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
    }

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

    if (comparators[spu->instructions[spu->read_bytes_amount] & ARGUMENT_MASK](intermediate_value_2, intermediate_value_1))
    {
        Jump(spu);
    }
    else
    {
        spu->read_bytes_amount += sizeof(uint8_t) + sizeof(int);
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
Call(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    if ((spu->instructions)[spu->read_bytes_amount] & ARGUMENT_MASK)
    {
        int intermediate_value = 0;

        if (StackPop(spu->spu_stack, &intermediate_value) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }

        spu->read_bytes_amount = (size_t) intermediate_value + sizeof(uint8_t) + sizeof(int);
    }
    else
    {
        if (StackPush(spu->spu_stack, (int) spu->read_bytes_amount) != 0)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }
        spu->read_bytes_amount += sizeof(uint8_t);
        spu->read_bytes_amount = (size_t) *((int*) (spu->instructions + spu->read_bytes_amount));
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

// ================= ARITHMETIC OPERATIONS =======================

static processor_functions_return_value_e StackAdd(spu_t* spu);
static processor_functions_return_value_e StackSub(spu_t* spu);
static processor_functions_return_value_e StackMul(spu_t* spu);
static processor_functions_return_value_e StackDiv(spu_t* spu);
static processor_functions_return_value_e StackSqrt(spu_t* spu);

processor_functions_return_value_e
StackDoOperation(spu_t* spu)
{
    processor_functions_return_value_e (*operations[])(spu_t*)  =
    {
        StackAdd,
        StackSub,
        StackMul,
        StackDiv,
        StackSqrt
    };

    PROCESSOR_VERIFY(spu);

    processor_functions_return_value_e output = operations[(spu->instructions)[spu->read_bytes_amount] & ARGUMENT_MASK](spu);

    PROCESSOR_VERIFY(spu);

    spu->read_bytes_amount += sizeof(uint8_t);

    return output;
}

static processor_functions_return_value_e
StackAdd(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    if ((StackPop(spu->spu_stack, &intermediate_value_1) != 0) || StackPop(spu->spu_stack, &intermediate_value_2) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (StackPush(spu->spu_stack, intermediate_value_2 + intermediate_value_1) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
StackSub(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    if ((StackPop(spu->spu_stack, &intermediate_value_1) != 0) || StackPop(spu->spu_stack, &intermediate_value_2) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (StackPush(spu->spu_stack, intermediate_value_2 - intermediate_value_1) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
StackMul(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    if ((StackPop(spu->spu_stack, &intermediate_value_1) != 0) || StackPop(spu->spu_stack, &intermediate_value_2) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (StackPush(spu->spu_stack, intermediate_value_2 * intermediate_value_1) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
StackDiv(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    if ((StackPop(spu->spu_stack, &intermediate_value_1) != 0) || StackPop(spu->spu_stack, &intermediate_value_2) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    if (intermediate_value_1 == 0)
    {
        return PROCESSOR_FUNCTION_RETURN_DIVISION_BY_ZERO;
    }

    if (StackPush(spu->spu_stack, intermediate_value_2 / intermediate_value_1) != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
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

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

//================== VERIFICATION =====================

static void
PrintRAMData(spu_t* spu)
{
    printf(YELLOW "__________________________________________________________________________________________________________\n"
                  "----------------------------------------------------RAM---------------------------------------------------\n" STANDARD);

    for (size_t index = 0; index < RAM_SIZE; index++)
    {
        if ((index % 8 == 0) && (index != 0))
        {
            printf(YELLOW "||\n" STANDARD);
        }

        printf(RED "[%3zu]" WHITE "%6d  " STANDARD, index % 1000,(spu->RAM)[index]);

    }

    printf(YELLOW "||\n" STANDARD);
    printf(YELLOW "----------------------------------------------------------------------------------------------------------\n" STANDARD);
}


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
        if ((spu->read_bytes_amount) == index)
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
    } while (index != spu->max_bytes_amount);
    printf(YELLOW "||\n" WHITE);

    printf(YELLOW "__________________________________________________________________________________________________________\n"
                  "-------------------------------------------------REGISTERS------------------------------------------------\n" STANDARD);

    for (size_t reg_index = 0; reg_index < PROCESSOR_REG_COUNT; reg_index++)
    {
        printf(YELLOW"||" RED "%s = " WHITE "%5d"  STANDARD, PROCESSORS_REG[reg_index], (spu->registers)[reg_index]);
        if ((reg_index + 1) % 8 == 0)
        {
            printf(YELLOW "||\n" WHITE);
        }
    }

    printf(YELLOW "||\n");

    StackDump(spu->spu_stack);

    #ifdef SHOW_RAM
    PrintRAMData(spu);
    #endif

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

