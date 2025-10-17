#ifndef READ_COMMANDS_H
#define READ_COMMANDS_H

#include <stdlib.h>
#include <stdint.h>

#include "compiler_commands.h"

const uint64_t COMPILER_VERSION = 3;

enum ReadErrorTypes
{
    READ_FILE_ERROR_TYPE_SUCCESS          = 0,
    READ_FILE_ERROR_TYPE_MEMORY_ERROR     = 1,
    READ_FILE_ERROR_TYPE_INPUT_ERROR      = 2,
    READ_FILE_ERROR_TYPE_READ_ERROR       = 3,
    READ_FILE_ERROR_TYPE_EMPTY_FILE_ERROR = 4,
    READ_FILE_ERROR_TYPE_SYSTEM_ERROR     = 5,
    READ_FILE_ERROR_TYPE_WRITE_ERROR      = 6
};

struct string_t
{
    char* string;
    size_t string_size;
};

ReadErrorTypes ReadFile(char** input_buffer, const char* input_name);
ReadErrorTypes WriteInFile(compiler_instructions_t* instructions, const char* write_name);
#endif //READ_COMMANDS_H
