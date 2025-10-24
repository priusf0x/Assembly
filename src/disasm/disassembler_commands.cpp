#include "disassembler_commands.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Assert.h"
#include "common_commands.h"

void
PrintHLT(uint8_t* instructions,
         size_t*  command_index,
         FILE*    output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    fprintf(output, "hlt\n");
    *command_index += sizeof(uint8_t);
}

void
PrintPush(uint8_t* instructions,
          size_t*  command_index,
          FILE*    output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    if ((instructions[*command_index] & USES_RAM) && (instructions[*command_index] & USES_INT))
    {
        *command_index += sizeof(uint8_t);
        fprintf(output, "push [%s + %d]\n",
                PROCESSORS_REG[instructions[*command_index - sizeof(uint8_t)] & REGISTER_MASK],
                *(int*) (instructions + *command_index));

        *command_index += sizeof(int);
    }
    else if (instructions[*command_index] & USES_INT)
    {
        *command_index += sizeof(uint8_t);
        fprintf(output, "push %d\n", *(int*) (instructions + *command_index));
        *command_index += sizeof(int);
    }
    else if (instructions[*command_index] & USES_RAM)
    {
        fprintf(output, "push [%s]\n", PROCESSORS_REG[((instructions[*command_index] & REGISTER_MASK))]);
        *command_index += sizeof(uint8_t);
    }
    else
    {
        fprintf(output, "push %s\n", PROCESSORS_REG[((instructions[*command_index] & REGISTER_MASK))]);
        *command_index += sizeof(uint8_t);
    }
}

void
PrintPop(uint8_t*    instructions,
         size_t* command_index,
         FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    if ((instructions[*command_index] & USES_RAM) && (instructions[*command_index] & USES_INT))
    {
        *command_index += sizeof(uint8_t);
        fprintf(output, "pop [%s + %d]\n",
                PROCESSORS_REG[instructions[*command_index - sizeof(uint8_t)] & REGISTER_MASK],
                *(int*) (instructions + *command_index));

        *command_index += sizeof(int);
    }
    else if (instructions[*command_index] & USES_RAM)
    {
        fprintf(output, "pop [%s]\n", PROCESSORS_REG[((instructions[*command_index] & REGISTER_MASK))]);
        *command_index += sizeof(uint8_t);
    }
    else
    {
        fprintf(output, "pop %s\n", PROCESSORS_REG[((instructions[*command_index] & REGISTER_MASK))]);
        *command_index += sizeof(uint8_t);
    }
}

void
PrintOut(uint8_t*    instructions,
         size_t* command_index,
         FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    if (instructions[*command_index] & ARGUMENT_MASK)
    {
        fprintf(output, "in\n");
    }
    else
    {
        fprintf(output, "out\n");
    }

    *command_index += sizeof(uint8_t);
}

void
PrintOperation(uint8_t*    instructions,
               size_t* command_index,
               FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    switch(instructions[*command_index] & ARGUMENT_MASK)
    {
        case 0b00000000:
            fprintf(output, "add\n");
            break;

        case 0b00000001:
            fprintf(output, "sub\n");
            break;

        case 0b00000010:
            fprintf(output, "mul\n");
            break;

        case 0b00000011:
            fprintf(output, "div\n");
            break;

        case 0b00000100:
            fprintf(output, "sqrt\n");
            break;

        default: break;
    }

    *command_index += sizeof(uint8_t);
}

void
PrintJump(uint8_t* instructions,
          size_t* command_index,
          FILE* output)
{
    switch(instructions[*command_index] & ARGUMENT_MASK)
    {
        case 0b00000000:
            fprintf(output, "jmp");
            break;

        case 0b00000001:
            fprintf(output, "ja");
            break;

        case 0b00000010:
            fprintf(output, "jae");
            break;

        case 0b00000011:
            fprintf(output, "jb");
            break;

        case 0b00000100:
            fprintf(output, "jbe");
            break;

        case 0b00000101:
            fprintf(output, "je");
            break;

        case 0b00000110:
            fprintf(output, "jne");
            break;

        default: break;
    }

    *command_index += sizeof(uint8_t);
    fprintf(output, " %d\n", *(int*) (instructions + *command_index));
    *command_index += sizeof(int);
}

void
PrintCall(uint8_t*    instructions,
          size_t* command_index,
          FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    if (instructions[*command_index] & ARGUMENT_MASK)
    {
        fprintf(output, "ret\n");
        *command_index += sizeof(uint8_t);
    }
    else
    {
        *command_index += sizeof(uint8_t);
        fprintf(output, "call %d\n", *(int*) (instructions + *command_index));
        *command_index += sizeof(int);
    }
}


void
PrintDraw(uint8_t*    instructions,
          size_t* command_index,
          FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    if (instructions[*command_index] & ARGUMENT_MASK)
    {
        fprintf(output, "drawb\n");
    }
    else
    {
        fprintf(output, "draw\n");
    }

    *command_index += sizeof(uint8_t);
}



