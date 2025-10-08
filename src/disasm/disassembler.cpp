#include <stdlib.h>
#include <stdio.h>

#include "disassembler_commands.h"

const char* ASSEMBLED_FILE_NAME = "compiled.obj";
const char* DISASSEMBLED_FILE_NAME = "disassembled.asm";

int main(void)
{
    size_t instructions_count = 0;

    FILE* assembled_file = fopen(ASSEMBLED_FILE_NAME, "rb");
    if (assembled_file == NULL)
    {
        printf("ASSEMBLED FILE WRITE ERROR.\n");
        exit(EXIT_FAILURE);
    }

    fread(&instructions_count , sizeof(int), 1, assembled_file);

    int* instructions = (int*) calloc(instructions_count, sizeof(int));
    fread(instructions, sizeof(int), instructions_count, assembled_file);

    if (fclose(assembled_file) != 0)
    {
        printf("ASSEMBLED FILE WRITE ERROR.\n");
        exit(EXIT_FAILURE);
    }

    FILE* disassembled_file = fopen(DISASSEMBLED_FILE_NAME, "w+");
    if (disassembled_file == NULL)
    {
        printf("DISASSEMBLED FILE WRITE ERROR.\n");
        exit(EXIT_FAILURE);
    }

    for(size_t command_index = 0; command_index < instructions_count; command_index++)
    {
        fprintf(disassembled_file, "%s", DISASSEMBLER_COMMANDS_ARRAY[instructions[command_index]].command_name);

        if (DISASSEMBLER_COMMANDS_ARRAY[instructions[command_index]].binary_handler != NULL)
        {
            (DISASSEMBLER_COMMANDS_ARRAY[instructions[command_index]].binary_handler)(instructions, &command_index, disassembled_file);
        }

        fprintf(disassembled_file, "\n");
    }

    if (fclose(disassembled_file) != 0)
    {
        printf("DISASSEMBLED FILE WRITE ERROR.\n");
        exit(EXIT_FAILURE);
    }

    free(instructions);

    return 0;
}
