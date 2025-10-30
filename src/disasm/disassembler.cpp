#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "Assert.h"
#include "disassembler_commands.h"
#include "common_commands.h"
#include "color.h"
#include "simple_parser.h"
#include "tools.h"

enum disassembler_return_e
{
    DISASSEMBLER_RETURN_SUCCESS,
    DISASSEMBLER_RETURN_READ_FLAG_ERROR,
    DISASSEMBLER_RETURN_ASSEMBLED_FILE_OPEN_ERROR,
    DISASSEMBLER_RETURN_DISASSEMBLED_FILE_OPEN_ERROR,
    DISASSEMBLER_RETURN_VERSION_MISMATCH
};

static disassembler_return_e ReadAssemblyFile(size_t* instructions_count, uint8_t** instructions);
static disassembler_return_e WriteDisassembledFile(size_t instructions_count, uint8_t* instructions);

const char* ASSEMBLED_FILE_NAME = "compiled.obj";
const char* DISASSEMBLED_FILE_NAME = "disassembled.asm";

const uint64_t DISASSEMBLER_VERSION = 4;

int
main(int                argc,
     const char* const* argv)
{
    int output = 0;
    size_t instructions_count = 0;
    uint8_t* instructions = NULL;

    if ((output = ReadFlags(argc, argv, &ASSEMBLED_FILE_NAME, &DISASSEMBLED_FILE_NAME)) != 0)
    {
        printf(RED "FLAG READ ERROR.\n" STANDARD);

        return output;
    }

    if ((output = ReadAssemblyFile(&instructions_count, &instructions)) != 0)
    {
        printf(RED "READ ASSEMBLY FILE ERROR.\n" STANDARD);

        return output;
    }

    if ((output = WriteDisassembledFile(instructions_count, instructions)) != 0)
    {

        return output;
    }

    free(instructions);

    return DISASSEMBLER_RETURN_SUCCESS;
}

static disassembler_return_e
ReadAssemblyFile(size_t*   instructions_count,
                 uint8_t** instructions)
{
    ASSERT(instructions_count);

    uint64_t compiler_version = 0;

    FILE* assembled_file = fopen(ASSEMBLED_FILE_NAME, "rb");
    if (assembled_file == NULL)
    {
        return DISASSEMBLER_RETURN_ASSEMBLED_FILE_OPEN_ERROR;
    }

    fread(&compiler_version , sizeof(uint64_t), 1, assembled_file);
    if (compiler_version != DISASSEMBLER_VERSION)
    {
        printf(RED "VERSION OF COMPILER IS MISMATCHING WITH VERSION OF DISASSEMBLER.\n" STANDARD);
        return DISASSEMBLER_RETURN_VERSION_MISMATCH;
    }

    fread(instructions_count , sizeof(size_t), 1, assembled_file);

    *instructions = (uint8_t*) calloc(*instructions_count, sizeof(uint8_t));
    fread(*instructions, sizeof(uint8_t), *instructions_count, assembled_file);

    if (fclose(assembled_file) != 0)
    {
        return DISASSEMBLER_RETURN_ASSEMBLED_FILE_OPEN_ERROR;
    }

    return DISASSEMBLER_RETURN_SUCCESS;
}

static disassembler_return_e
WriteDisassembledFile(size_t   instructions_count,
                      uint8_t* instructions)
{
    ASSERT(instructions);

    FILE* disassembled_file = fopen(DISASSEMBLED_FILE_NAME, "w+");
    if (disassembled_file == NULL)
    {
        return DISASSEMBLER_RETURN_DISASSEMBLED_FILE_OPEN_ERROR;
    }

    int command_number = 0;

    for(size_t command_index = 0; command_index < instructions_count;)
    {
        command_number = TranslateCommandNumber(instructions, &command_index);
        (DISASSEMBLER_COMMANDS_ARRAY[command_number].binary_handler)
        (instructions, &command_index, disassembled_file);
    }

    if (fclose(disassembled_file) != (int) DISASSEMBLER_RETURN_SUCCESS)
    {
        return DISASSEMBLER_RETURN_DISASSEMBLED_FILE_OPEN_ERROR;
    }

    return DISASSEMBLER_RETURN_SUCCESS;
}
