#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "disassembler_commands.h"
#include "common_commands.h"
#include "color.h"
#include "simple_parser.h"

enum disassembler_main_return_e
{
    DISASSEMBLER_MAIN_RETURN_SUCCESS,
    DISASSEMBLER_MAIN_RETURN_READ_FLAG_ERROR,
    DISASSEMBLER_MAIN_RETURN_ASSEMBLED_FILE_OPEN_ERROR,
    DISASSEMBLER_MAIN_RETURN_DISASSEMBLED_FILE_OPEN_ERROR,
    DISASSEMBLER_MAIN_RETURN_VERSION_MISMATCH
};

const char* ASSEMBLED_FILE_NAME = "compiled.obj";
const char* DISASSEMBLED_FILE_NAME = "disassembled.asm";

const uint64_t DISASSEMBLER_VERSION = 4;

int
main(int                argc,
     const char* const* argv)
{
    size_t instructions_count = 0;
    uint64_t compiler_version = 0;

    if (ReadFlags(argc, argv, &ASSEMBLED_FILE_NAME, &DISASSEMBLED_FILE_NAME) != (int) DISASSEMBLER_MAIN_RETURN_SUCCESS)
    {
        printf(RED "FLAG READ ERROR.\n" STANDARD);

        return DISASSEMBLER_MAIN_RETURN_READ_FLAG_ERROR;
    }

    FILE* assembled_file = fopen(ASSEMBLED_FILE_NAME, "rb");
    if (assembled_file == NULL)
    {
        printf(RED "ASSEMBLED FILE WRITE ERROR.\n" STANDARD);

        return DISASSEMBLER_MAIN_RETURN_ASSEMBLED_FILE_OPEN_ERROR;
    }

    fread(&compiler_version , sizeof(uint64_t), 1, assembled_file);
    if (compiler_version != DISASSEMBLER_VERSION)
    {
        printf(RED "VERSION OF COMPILER IS MISMATCHING WITH VERSION OF DISASSEMBLER.\n" STANDARD);
        return DISASSEMBLER_MAIN_RETURN_VERSION_MISMATCH;
    }

    fread(&instructions_count , sizeof(uint64_t), 1, assembled_file);
    instructions_count -= sizeof(uint64_t);

    uint8_t* instructions = (uint8_t*) calloc(instructions_count, sizeof(uint8_t));
    fread(instructions, sizeof(uint8_t), instructions_count, assembled_file);

    if (fclose(assembled_file) != (int) DISASSEMBLER_MAIN_RETURN_SUCCESS)
    {
        printf(RED "ASSEMBLED FILE WRITE ERROR.\n" STANDARD);

        return DISASSEMBLER_MAIN_RETURN_ASSEMBLED_FILE_OPEN_ERROR;
    }

    FILE* disassembled_file = fopen(DISASSEMBLED_FILE_NAME, "w+");
    if (disassembled_file == NULL)
    {
        free(instructions);
        printf(RED "DISASSEMBLED FILE WRITE ERROR.\n" STANDARD);

        return DISASSEMBLER_MAIN_RETURN_DISASSEMBLED_FILE_OPEN_ERROR;
    }

    for(size_t command_index = 0; command_index < instructions_count;)
    {
        (DISASSEMBLER_COMMANDS_ARRAY[(instructions[command_index] & COMMAND_MASK) >> 4].binary_handler)(instructions, &command_index, disassembled_file);
    }

    if (fclose(disassembled_file) != (int) DISASSEMBLER_MAIN_RETURN_SUCCESS)
    {
        free(instructions);
        printf(RED "DISASSEMBLED FILE WRITE ERROR.\n" STANDARD);

        return DISASSEMBLER_MAIN_RETURN_DISASSEMBLED_FILE_OPEN_ERROR;
    }

    free(instructions);

    return DISASSEMBLER_MAIN_RETURN_SUCCESS;
}
