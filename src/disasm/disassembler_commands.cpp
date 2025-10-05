#include "disassembler_commands.h"

#include <stdlib.h>
#include <stdio.h>

#include "Assert.h"

void
PrintPushArg(int* instructions,
             size_t* command_index,
             FILE* output)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    ASSERT(instructions != NULL);
    (*command_index)++;
    fprintf(output, " %d", instructions[*command_index]);
}
