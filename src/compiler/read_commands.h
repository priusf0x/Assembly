#ifndef READ_COMMANDS
#define READ_COMMANDS

#include <stdlib.h>

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

struct string_t
{
    char* string;
    size_t string_size;
};

ReadErrorTypes ReadFile(char** input_buffer, string_t** array_of_strings, size_t* str_count, const char* input_name);
void           EnterData(string_t* array_of_strings, size_t str_count, char*  input_buffer);

#endif //read_commands
