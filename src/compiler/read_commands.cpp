#include "read_commands.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>

#include "Assert.h"
#include "color.h"
#include "labels.h"
#include "tools.h"
#include "common_commands.h"

ReadErrorTypes
ReadFile(char**      input_buffer,
         const char* input_name)
{
    ASSERT(input_buffer != NULL);
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
        *input_buffer = NULL;
        fclose(file_input);
        return READ_FILE_ERROR_TYPE_READ_ERROR;
    }

    if (fclose(file_input) != 0)
    {
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
        *input_buffer = NULL;
        return READ_FILE_ERROR_TYPE_EMPTY_FILE_ERROR;
    }

    return READ_FILE_ERROR_TYPE_SUCCESS;
}

ReadErrorTypes
WriteInFile(compiler_instructions_t* instructions,
            const char*              write_name)
{
    FILE* compiled_file = fopen(write_name, "wb+");
    if (compiled_file == NULL)
    {
        return READ_FILE_ERROR_TYPE_WRITE_ERROR;
    }

    fwrite(instructions->instructions_array , sizeof(int), instructions->instructions_count + 1, compiled_file);
    fprintf(compiled_file, "\nHere was pr1usf0x.\n");

    if (fclose(compiled_file) != 0)
    {
        return READ_FILE_ERROR_TYPE_WRITE_ERROR;
    }

    return READ_FILE_ERROR_TYPE_SUCCESS;
}

//=================== READ COMMANDS FUNCTIONS ========================

compiler_return_e
TranslateCode(char*                    input_buffer,
              compiler_instructions_t* instructions)
{
    char* input_command = input_buffer;
    size_t command_index = 0;
    label_tabular_t* label_tabular = InitialiseLabelTabular();

    while (*(input_command) != '\0')
    {
        if (CheckIfLabel(input_command))
        {
            char* label_name = input_command;
            *(SkipNotSpaces(label_name) - 1) = '\0';

            if (UseLabel(label_name, instructions,  label_tabular) != LABEL_INSTRUCTION_RETURN_SUCCESS)
            {
                printf("EMPTY LABEL OR REUSED TWICE.\n");
                DestroyLabelTabular(label_tabular);
                return COMPILER_RETURN_LABEL_ERROR;
            }

            LabelTabularDump(label_tabular);

            input_command = SkipSpaces(SkipNotSpaces(input_command)) + 1;
        }

        compiler_return_e output = ReadCommand(&input_command, instructions);
        input_command = SkipSpaces(SkipNotSpaces(input_command)) + 1;
        command_index++;

        if (output == COMPILER_RETURN_INCORRECT_COMMAND)
        {
            printf("INCORRECT COMMAND %zu.\n", command_index);
            DestroyLabelTabular(label_tabular);
            return COMPILER_RETURN_INCORRECT_COMMAND;
        }
        if (output == COMPILER_RETURN_INVALID_SYNTAX)
        {
            printf("INCORRECT SYNTAX IN COMMAND %zu.\n", command_index);
            DestroyLabelTabular(label_tabular);
            return COMPILER_RETURN_INVALID_SYNTAX;
        }
    }

    (instructions->instructions_array)[0] = (int) instructions->instructions_count;

    DestroyLabelTabular(label_tabular);

    return COMPILER_RETURN_SUCCESS;
}



compiler_return_e
ReadCommand(char**                   input_command,
            compiler_instructions_t* instructions)
{
    commands_e read_command = COMMAND_HLT;

    size_t command_size = (size_t) (SkipNotSpaces(*input_command) - *input_command);
    bool find_flag = false;
    for (size_t index = 0; index < COMMANDS_COUNT; index++)
    {
        if ((COMPILER_COMMANDS_ARRAY[index]).command_name == NULL)
        {
            continue;
        }
        if (strncmp(*input_command, COMPILER_COMMANDS_ARRAY[index].command_name, command_size) == 0)
        {
            read_command = (COMPILER_COMMANDS_ARRAY[index]).return_value;
            if (PutInstruction(read_command, instructions) != 0)
            {
                return COMPILER_RETURN_INCORRECT_COMMAND;
            }
            find_flag = true;

            break;
        }
    }

    if (!find_flag)
    {
        return COMPILER_RETURN_INCORRECT_COMMAND;
    }

    if (COMPILER_COMMANDS_ARRAY[read_command].handler != NULL)
    {
        *input_command = SkipNotSpaces(*input_command);
        *input_command = SkipSpaces(*input_command);

        compiler_return_e output = (COMPILER_COMMANDS_ARRAY[read_command].handler)(input_command, instructions);
        if (output != COMPILER_RETURN_VALID_SYNTAX)
        {
            return output;
        }
    }

    return COMPILER_RETURN_SUCCESS;
}


// ================ HANDLERS ====================

compiler_return_e
ReadPushArgument(char**                   input_command,
                 compiler_instructions_t* instructions)
{
    ASSERT(*input_command != NULL);
    ASSERT(instructions != NULL);

    if (!IsStrNum(*input_command))
    {
        bool find_flag = false;

        for (int register_number = 0; register_number < PROCESSOR_REG_COUNT; register_number++)
        {
            if (strncmp(PROCESSORS_REG[register_number], *input_command, strlen(PROCESSORS_REG[register_number])) == 0)
            {
                // printf("%s", PROCESSORS_REG[register_number]);
                if (PutInstruction(register_number, instructions) == 1)
                {
                    return COMPILER_RETURN_INCORRECT_COMMAND;
                }
                find_flag = true;
                (instructions->instructions_array)[instructions->instructions_count - 1] = COMMAND_PUSH_IN_REG;

                break;
            }
        }
        if (!find_flag)
        {
            return COMPILER_RETURN_INVALID_SYNTAX;
        }
    }
    else if (PutInstruction(atoi(*input_command), instructions) == 1)
    {
        return COMPILER_RETURN_INCORRECT_COMMAND;
    }

    return COMPILER_RETURN_VALID_SYNTAX;
}

compiler_return_e
ReadPopArgument(char**                    input_command,
                compiler_instructions_t*  instructions)
{
    ASSERT(input_command != NULL);
    ASSERT(instructions != NULL);

    bool find_flag = false;

    for (int register_number = 0; register_number < PROCESSOR_REG_COUNT; register_number++)
    {
        if (strncmp(PROCESSORS_REG[register_number], *input_command, strlen(PROCESSORS_REG[register_number])) == 0)
        {
            // printf("%s", PROCESSORS_REG[register_number]);
            if (PutInstruction(register_number, instructions) == 1)
            {
                return COMPILER_RETURN_INCORRECT_COMMAND;
            }

            find_flag = true;
            break;
        }
    }
    if (!find_flag)
    {
        // printf("huesos");
        return COMPILER_RETURN_INVALID_SYNTAX;
    }

    return COMPILER_RETURN_VALID_SYNTAX;
}
