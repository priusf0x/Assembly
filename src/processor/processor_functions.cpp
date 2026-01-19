#include "processor_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

#include "assert.h"
#include "color.h"
#include "logger.h"
#include "stack.h"
#include "tools.h"
#include "common_commands.h"

// #define NDEBUG
#define VIDEO_PLAY
// #define SHOW_RAM

const size_t START_STACK_SIZE = 8;
const uint64_t PROCESSOR_VERSION = 5;
const size_t SCREEN_SIZE_X = 98;
const size_t SCREEN_SIZE_Y = 36;
const size_t RAM_SIZE = 10000;

static int* ArgGetInt(spu_t* spu);

#define NEXT_INSTRUCTION() spu->read_bytes_amount += sizeof(uint8_t)
#define SKIP_INT() spu->read_bytes_amount += sizeof(int)
#define GET_INT() (int*) (spu->instructions + spu->read_bytes_amount)
#define PUSH_RET(X) if (StackPush(spu->spu_stack, (X)) != 0) \
                    {\
                        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;\
                    }
#define POP_RET(X)  if (StackPop(spu->spu_stack, (X)) != 0) \
                    {\
                        return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;\
                    }
#define ARG_GET_INT() ArgGetInt(spu);

static int*
ArgGetInt(spu_t* spu)
{
    // FIXME: При добавлении новой команды, должно быть достаточно передать в эту функцию некоторый enum,
    // FIXME: который будет описывать как воспринимать агрумент (kak PUSH ili kak POP) FIXME:

    uint8_t* instructions = spu->instructions;
    if ((instructions[spu->read_bytes_amount - 1] & USES_RAM)
        && (instructions[spu->read_bytes_amount - 1] & ADD_TO_REGI))
    {
        uint8_t spu_reg = (spu->instructions)[spu->read_bytes_amount - 1]
                            & REGISTER_MASK;
        int* read_integer = GET_INT();
        SKIP_INT();
        return spu->RAM + spu->registers[spu_reg] + *read_integer;
    }
    else if (instructions[spu->read_bytes_amount - 1] & USES_RAM)
    {
        uint8_t spu_reg = (spu->instructions)[spu->read_bytes_amount - 1]
                            & REGISTER_MASK;
        return spu->RAM + spu->registers[spu_reg];
    }
    else if (instructions[spu->read_bytes_amount - 1] & USES_INT)
    {
        int* read_integer = GET_INT();
        SKIP_INT();
        return read_integer;
    }
    else
    {
        uint8_t spu_reg = (spu->instructions)[spu->read_bytes_amount - 1]
                           & REGISTER_MASK;
        return spu->registers + spu_reg;
    }
}

static void PrintRAMData(spu_t* spu);

processor_functions_return_value_e
InitializeSPU(spu_t*      spu,
              const char* assembled_file_name)
{
    // FIXME: make error handling correct
    if (StackInit(&(spu->spu_stack), START_STACK_SIZE, "SPU stack") != 0)
    {
        return PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_INIT_STACK;
    }

    FILE* assembled_file = fopen(assembled_file_name, "rb");
    if (assembled_file == NULL)
    {
        StackDestroy(spu->spu_stack);
        return PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_READ_INSTRUCTIONS;
    }

    uint64_t compiler_version = 0;
    fread(&compiler_version , sizeof(uint64_t), 1, assembled_file);
    if (compiler_version != PROCESSOR_VERSION)
    {
        StackDestroy(spu->spu_stack);
        // FIXME: write errors to stderr
        fprintf(stderr , RED "VERSION OF COMPILER IS MISMATCHING WITH VERSION OF PROCESSOR.\n" STANDARD);
        return PROCESSOR_FUNCTION_RETURN_VERSIONS_MISMATCH;
    }

    size_t max_bytes_amount = 0;
    fread(&(max_bytes_amount) , sizeof(size_t), 1, assembled_file);

    spu->max_bytes_amount = max_bytes_amount;

    spu->instructions = (uint8_t*) calloc(max_bytes_amount, sizeof(uint8_t));
    if (spu->instructions == NULL)
    {
        StackDestroy(spu->spu_stack);
        fclose(assembled_file);
        return PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR;
    }

    fread(spu->instructions, sizeof(uint8_t), (size_t) max_bytes_amount, assembled_file);

    if (fclose(assembled_file) != 0)
    {
        StackDestroy(spu->spu_stack);
        return PROCESSOR_FUNCTION_RETURN_VALUE_FAILED_TO_READ_INSTRUCTIONS;
    }

    spu->read_bytes_amount = 0;

    // FIXME: make register array static
    spu->registers = (int*) calloc(PROCESSOR_REG_COUNT, sizeof(int));
    if (spu->registers == NULL)
    {
        StackDestroy(spu->spu_stack);
        return PROCESSOR_FUNCTION_RETURN_VALUE_MEMORY_ERROR;
    }

    spu->RAM = (int*) calloc(RAM_SIZE, sizeof(int));
    if (spu->RAM == NULL)
    {
        StackDestroy(spu->spu_stack);
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
    //  while (command = GetCommand(&spu)) {
    //      Execute(command);
    //  }

    assert(spu);

    size_t cmd_opcode = TranslateCommandNumber(spu->instructions, &(spu->read_bytes_amount));
    processor_functions_return_value_e processor_error = PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;

    while (cmd_opcode /*FIXME: условия на HLT должно быть тут*/)
    {
        spu->read_bytes_amount++;
        if (PROCESSOR_COMMANDS_ARRAY[cmd_opcode].command_function != NULL) // перенести
        {
            processor_error = PROCESSOR_COMMANDS_ARRAY[cmd_opcode].command_function (spu);
            if (processor_error != PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS)
            {
                ProcessorDump(spu);
                printf(RED "ERROR NUMBER:...............................%d.\n"
                        RED "P" ORANGE "i" YELLOW "z" GREEN "d" BLUE "e" PURPLE "c " WHITE
                       "has happened...\n" STANDARD, processor_error);

                return processor_error;
            }
        }

        #ifndef NDEBUG
            ProcessorDump(spu);
        #endif

        if (spu->read_bytes_amount >= spu->max_bytes_amount)
        {
            return PROCESSOR_FUNCTION_RETURN_OUT_OF_MEMORY;
        }

        cmd_opcode = TranslateCommandNumber(spu->instructions, &(spu->read_bytes_amount));
    }

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

//================ COMMANDS_FUNCTIONS =========================ь

processor_functions_return_value_e
DrawScreen(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    fprintf(stdout, "\e[1;1H\e[2J"); //clear screen
    if (spu->instructions[spu->read_bytes_amount - 1] & ARGUMENT_MASK)
    {
        fwrite((uint8_t*) spu->RAM, sizeof(uint8_t), SCREEN_SIZE_X * SCREEN_SIZE_Y, stdout);
    }
    else
    {
        fwrite(spu->RAM, sizeof(int), SCREEN_SIZE_X * SCREEN_SIZE_Y, stdout);
    }

    #ifdef VIDEO_PLAY
        usleep(20000);
    #endif

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e StackAdd(spu_t* spu);
const uint8_t ADD_PUSH_BYTE_CODE = 0b01000000 | ADD_TO_REGI;

processor_functions_return_value_e
StackCommandPush(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    bool add_flag = false;
    if ((spu->instructions[spu->read_bytes_amount - 1] & (~REGISTER_MASK))
         == ADD_PUSH_BYTE_CODE)
    {
        add_flag = true;
    }

    int* ptr = ARG_GET_INT();
    PUSH_RET(*ptr);
    // meow rax + 5

    if (add_flag)
    {
        PUSH_RET(*GET_INT());
        StackAdd(spu);
        SKIP_INT();
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

const uint8_t ADD_POP_BYTE_CODE = 0b10000000 | ADD_TO_REGI;

processor_functions_return_value_e
StackCommandPop(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    bool add_flag = false;
    uint8_t reg_number = 0;
    if ((spu->instructions[spu->read_bytes_amount - 1] & (~REGISTER_MASK))
         == ADD_POP_BYTE_CODE)
    {
        add_flag = true;
        reg_number = spu->instructions[spu->read_bytes_amount - 1] & REGISTER_MASK;
    }

    int* ptr = ARG_GET_INT();
    POP_RET(ptr);

    if (add_flag)
    {
        int add_value = *GET_INT();
        spu->registers[reg_number] += add_value;
        SKIP_INT();
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
StackInOut(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value = 0;

    if (spu->instructions[spu->read_bytes_amount - 1] & ARGUMENT_MASK)
    {
        if (scanf("%d", &intermediate_value) != 1)
        {
            return PROCESSOR_FUNCTION_RETURN_STACK_ERROR;
        }

        PUSH_RET(intermediate_value);
    }
    else
    {
        POP_RET(&intermediate_value);

        printf(GREEN "OUT-VALUE" WHITE " - %d\n" STANDARD, intermediate_value);
    }

    PROCESSOR_VERIFY(spu);

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
    spu->read_bytes_amount = (size_t) *GET_INT();

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

    if (!(spu->instructions[spu->read_bytes_amount - 1] & ARGUMENT_MASK))
    {
        Jump(spu);
        return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
    }

    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    POP_RET(&intermediate_value_1);
    POP_RET(&intermediate_value_2);

    if (comparators[spu->instructions[spu->read_bytes_amount - 1]
        & ARGUMENT_MASK](intermediate_value_2, intermediate_value_1))
    {
        Jump(spu);
    }
    else
    {
        SKIP_INT();
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

processor_functions_return_value_e
Call(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    if ((spu->instructions)[spu->read_bytes_amount - 1] & ARGUMENT_MASK)
    {
        int intermediate_value = 0;

        POP_RET(&intermediate_value);

        spu->read_bytes_amount = (size_t) intermediate_value;
        NEXT_INSTRUCTION();
        SKIP_INT();
    }
    else
    {
        PUSH_RET((int) (spu->read_bytes_amount - 1));

        spu->read_bytes_amount = (size_t) *GET_INT();
    }

    PROCESSOR_VERIFY(spu);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

// ================= ARITHMETIC OPERATIONS =======================

// static processor_functions_return_value_e StackAdd(spu_t* spu);
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

    processor_functions_return_value_e output =
    operations[(spu->instructions)[spu->read_bytes_amount - 1] & ARGUMENT_MASK](spu);

    PROCESSOR_VERIFY(spu);

    return output;
}

static processor_functions_return_value_e
StackAdd(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    POP_RET(&intermediate_value_1);
    POP_RET(&intermediate_value_2);

    PUSH_RET(intermediate_value_2 + intermediate_value_1)

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
StackSub(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    POP_RET(&intermediate_value_1);
    POP_RET(&intermediate_value_2);

    PUSH_RET(intermediate_value_2 - intermediate_value_1);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
StackMul(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    POP_RET(&intermediate_value_1);
    POP_RET(&intermediate_value_2);

    PUSH_RET(intermediate_value_2 * intermediate_value_1);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
StackDiv(spu_t* spu)
{
    int intermediate_value_1 = 0;
    int intermediate_value_2 = 0;

    POP_RET(&intermediate_value_1);
    POP_RET(&intermediate_value_2);

    if (intermediate_value_1 == 0)
    {
        return PROCESSOR_FUNCTION_RETURN_DIVISION_BY_ZERO;
    }

    PUSH_RET(intermediate_value_2 / intermediate_value_1);

    return PROCESSOR_FUNCTION_RETURN_VALUE_SUCCESS;
}

static processor_functions_return_value_e
StackSqrt(spu_t* spu)
{
    PROCESSOR_VERIFY(spu);

    int intermediate_value = 0;

    POP_RET(&intermediate_value);

    if (intermediate_value < 0)
    {
        return PROCESSOR_FUNCTION_RETURN_SQRT_ERROR;
    }

    PUSH_RET((int) sqrt(intermediate_value));

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
        printf(YELLOW"||" RED "%s = " WHITE "%5d"  STANDARD,
               PROCESSORS_REG[reg_index], (spu->registers)[reg_index]);
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



