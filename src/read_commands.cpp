#include "read_commands.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>

#include "Assert.h"
#include "color.h"
#include "stack.h"
#include "calculator.h"
#include "commands.h"

calculator_commands_e
ReadCommand(string_t input_command)
{
    size_t count = 0;
    char character = 0;

    calculator_commands_e read_command = CALCULATOR_COMMAND_INCORRECT_COMMAND;

    while (!isspace(character = (input_command.string)[count]) && (character != ';' ))
    {
        count++;
    }

    for (size_t index = 0; index < commands_count; index++)
    {
        if ((commands_array[index]).command_name == NULL)
        {
            continue;
        }
        if (strncmp(input_command.string, commands_array[index].command_name, count) == 0)
        {
            read_command = (commands_array[index]).return_value;
            break;
        }
    }

    return read_command;
}

void
ReadCoefficient(stack_t*               swag,
                calculator_commands_e* input_command)
{
    ASSERT(swag != NULL);
    ASSERT(input_command != NULL);

    value_type coefficient = 0;

    if (scanf("%d", &coefficient) != 1)
    {
        ClearBuffer();
        *input_command = CALCULATOR_COMMAND_INVALID_SYNTAX;
        return;
    }

    CheckIfSpaces(input_command);
    if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
    {
        return;
    }

    stack_function_errors_e return_error = StackPush(swag, coefficient);
    if (return_error != STACK_FUNCTION_SUCCESS)
    {
        ClearBuffer();
        LOG_FUNCTION_ERROR(return_error);
        *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
        return;
    }

    *input_command = CALCULATOR_COMMAND_START;
}

bool
ClearBuffer()
{
    bool flag = false;
    int character = 0;
    while ((character = getchar()) != '\n' && character != EOF)
    {
        if (!isspace(character))
        {
            flag = true;
        }
    }

    return flag;
}

void
CheckIfSpaces(calculator_commands_e* expected_state)
{
    if (ClearBuffer())
    {
        *expected_state = CALCULATOR_COMMAND_INVALID_SYNTAX;
    }
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

    printf("%d\n", EnterData(*array_of_strings, *str_count, *input_buffer));

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
        if ((strchr(current_pointer, ';') == NULL) || (strchr(current_pointer, ';') > strchr(current_pointer, '\n')))
        {
            return COMPILER_ERROR_TYPES_SKIPPED_END_SYMBOL;
        }
        (array_of_strings[counter]).string_size = (size_t) (strchr(current_pointer, ';') - current_pointer);
        (array_of_strings[counter]).string = current_pointer;

        current_pointer = strchr(current_pointer, '\n') + 1;
        ((array_of_strings[counter]).string)[(array_of_strings[counter]).string_size] = '\0';
        printf("%s", (array_of_strings[counter]).string);

        counter++;
    }

    return COMPILER_ERROR_TYPES_CORRECT;
}
