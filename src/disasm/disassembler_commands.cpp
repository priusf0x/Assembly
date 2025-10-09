#include "disassembler_commands.h"

#include <stdlib.h>
#include <stdio.h>

#include "Assert.h"
#include "common_commands.h"

void
PrintPushArg(int*    instructions,
             size_t* command_index,
             FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    if (instructions[*command_index] == COMMAND_PUSH_IN_REG)
    {
        (*command_index)++;
        fprintf(output, " %s", PROCESSORS_REG[instructions[*command_index]]);
    }
    else
    {
        (*command_index)++;
        fprintf(output, " %d", instructions[*command_index]);
    }
}

void
PrintPopArg(int*    instructions,
             size_t* command_index,
             FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    (*command_index)++;
    fprintf(output, " %s", PROCESSORS_REG[instructions[*command_index]]);
}

