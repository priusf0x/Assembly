#include "disassembler_commands.h"

#include <stdlib.h>
#include <stdio.h>

#include "Assert.h"
#include "common_commands.h"

void
PrintPushPopArg(int*    instructions,
             size_t* command_index,
             FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    if (instructions[*command_index] == COMMAND_PUSH_FROM_REG || instructions[*command_index] == COMMAND_POP)
    {
        (*command_index)++;
        fprintf(output, " %s", PROCESSORS_REG[instructions[*command_index]]);
    }
    else if (instructions[*command_index] == COMMAND_PUSH_FROM_MEMORY || instructions[*command_index] == COMMAND_POP_TO_MEMORY)
    {
        (*command_index)++;
        fprintf(output, " [%s]", PROCESSORS_REG[instructions[*command_index]]);
    }
    else if (instructions[*command_index] == COMMAND_PUSH)
    {
        (*command_index)++;
        fprintf(output, " %d", instructions[*command_index]);
    }
}

void
PrintJumpArg(int*    instructions,
             size_t* command_index,
             FILE*   output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);

    (*command_index)++;
    fprintf(output, " %d", instructions[*command_index]);
}




