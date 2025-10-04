#ifndef READ_COMMANDS
#define READ_COMMANDS

#include "commands.h"

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

commands_e         ReadCommand(char* input_command);
ReadErrorTypes     ReadFile(char** input_buffer, string_t** array_of_strings, size_t* str_count, const char* input_name);
CompilerErrorTypes EnterData(string_t* array_of_strings, size_t str_count, char*  input_buffer);
commands_e         ReadIntValue(char* input_command);

#endif //read_commands
