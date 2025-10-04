#ifndef READ_COMMANDS
#define READ_COMMANDS

#include <stdlib.h>

#include "commands.h"

struct instructions_t
{
    size_t instructions_count;
    size_t instructions_size;
    int* instructions_array;
};

enum ReadErrorTypes
{
    READ_FILE_ERROR_TYPE_SUCCESS          = 0,
    READ_FILE_ERROR_TYPE_MEMORY_ERROR     = 1,
    READ_FILE_ERROR_TYPE_INPUT_ERROR      = 2,
    READ_FILE_ERROR_TYPE_READ_ERROR       = 3,
    READ_FILE_ERROR_TYPE_EMPTY_FILE_ERROR = 4,
    READ_FILE_ERROR_TYPE_SYSTEM_ERROR     = 5
};

enum CompilerErrorTypes
{
    COMPILER_ERROR_TYPES_CORRECT,
    COMPILER_ERROR_TYPES_SKIPPED_END_SYMBOL,
};

struct string_t
{
    char* string;
    size_t string_size;
};

commands_e         ReadCommand(char* input_command, instructions_t* instructions);
ReadErrorTypes     ReadFile(char** input_buffer, string_t** array_of_strings, size_t* str_count, const char* input_name);
CompilerErrorTypes EnterData(string_t* array_of_strings, size_t str_count, char*  input_buffer);
commands_e         ReadIntValue(char* input_command, instructions_t* instructions);

#endif //read_commands
