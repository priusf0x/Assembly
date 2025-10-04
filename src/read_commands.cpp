#include "read_commands.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>

#include "commands.h"
#include "Assert.h"
#include "color.h"
#include "tools.h"

commands_e
ReadCommand(char* input_command)
{
    printf("%s\n", input_command);
    commands_e read_command = COMMAND_INCORRECT_COMMAND;
    if (input_command[0] == '\0')
    {
        return COMMAND_EMPTY_COMMAND;
    }

    size_t command_size = size_t(SkipNotSpaces(input_command) - input_command);

    for (size_t index = 0; index < commands_count; index++)
    {
        if ((COMMANDS_ARRAY[index]).command_name == NULL)
        {
            continue;
        }
        if (strncmp(input_command, COMMANDS_ARRAY[index].command_name, command_size) == 0)
        {
            read_command = (COMMANDS_ARRAY[index]).return_value;
            break;
        }
    }

    printf("%d ", read_command);

    input_command = SkipNotSpaces(input_command);

    for (size_t count = 0; count < COMMANDS_ARRAY[read_command].number_of_arguments; count++)
    {
        input_command = SkipSpaces(input_command);

        if (*input_command == '\0')
        {
            return COMMAND_INVALID_SYNTAX;
        }

        if (ReadIntValue(input_command) == COMMAND_INVALID_SYNTAX)
        {
            return COMMAND_INVALID_SYNTAX;
        }

        input_command = SkipNotSpaces(input_command);
    }

    printf("\n");

    if (*SkipSpaces(input_command) != '\0')
    {
        return COMMAND_INVALID_SYNTAX;
    }

    return read_command;
}

commands_e
ReadIntValue(char* input_command)
{
    if (!IsStrNum(input_command))
    {
        return COMMAND_INVALID_SYNTAX;
    }

    printf("%d", atoi(input_command));

    return COMMAND_VALID_SYNTAX;
}

ReadErrorTypes
ReadFile(char**      input_buffer,
         string_t**  array_of_strings,
         size_t*     str_count,
         const char* input_name)
{
    ASSERT(input_buffer != NULL);
    ASSERT(array_of_strings != NULL);
    ASSERT(str_count != NULL);
    ASSERT(input_name != NULL);

    struct stat file_stat = {};

    if (stat(input_name, &file_stat) != 0)
    {
        return READ_FILE_ERROR_TYPE_SYSTEM_ERROR;
    }

    size_t char_number = (size_t) (file_stat.st_size);

    FILE* file_input = fopen(input_name, "r");
    if (file_input == NULL)
    {
        return READ_FILE_ERROR_TYPE_READ_ERROR;
    }

    *input_buffer = (char*) calloc(char_number + 2, sizeof(char));//1 for \0 another one fo adding \n
    if (*input_buffer == NULL)
    {
        fclose(file_input);
        return READ_FILE_ERROR_TYPE_MEMORY_ERROR;
    }

    size_t read_count = fread(*input_buffer, sizeof(char), char_number, file_input);
    if (read_count != char_number)
    {
        free(*input_buffer);
        *input_buffer = NULL;
        fclose(file_input);
        return READ_FILE_ERROR_TYPE_READ_ERROR;
    }

    if (fclose(file_input) != 0)
    {
        free(*input_buffer);
        *input_buffer = NULL;
        return READ_FILE_ERROR_TYPE_READ_ERROR;
    }
    memset(input_buffer + read_count + 1, 0, char_number - read_count);
    if (read_count > 0)
    {
        if ((*input_buffer)[read_count - 1] != '\n')
        {
            (*input_buffer)[read_count] = '\n';
        }
    }
    else
    {
        free(*input_buffer);
        *input_buffer = NULL;
        return READ_FILE_ERROR_TYPE_EMPTY_FILE_ERROR;
    }

    *str_count = CountCharInStr(';', *input_buffer);
    *array_of_strings = (string_t*) calloc(*str_count, sizeof(string_t));

    if (*array_of_strings == NULL)
    {
        free(*input_buffer);
        *input_buffer = NULL;
        return READ_FILE_ERROR_TYPE_MEMORY_ERROR;
    }

    EnterData(*array_of_strings, *str_count, *input_buffer);

    return READ_FILE_ERROR_TYPE_SUCCESS;
}

CompilerErrorTypes
EnterData(string_t* array_of_strings,
          size_t str_count,
          char*  input_buffer)
{
    ASSERT(array_of_strings != NULL);
    ASSERT(input_buffer != NULL);

    char* current_pointer = input_buffer;
    size_t counter = 0;

    while (counter < str_count)
    {
        if (strchr(current_pointer, ';') > strchr(current_pointer, '\n'))
        {
            current_pointer = strchr(current_pointer, '\n') + 1;
            continue;
        }
        (array_of_strings[counter]).string_size = (size_t) (strchr(current_pointer, ';') - current_pointer);
        (array_of_strings[counter]).string = current_pointer;

        current_pointer = strchr(current_pointer, '\n') + 1;
        ((array_of_strings[counter]).string)[(array_of_strings[counter]).string_size] = '\0';
        // printf("%s", (array_of_strings[counter]).string);

        counter++;
    }

    return COMPILER_ERROR_TYPES_CORRECT;
}
