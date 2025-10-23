#include "read_commands.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdint.h>

#include "Assert.h"
#include "color.h"
#include "tools.h"
#include "common_commands.h"

static int  PutInstruction(size_t index_in_table, compiler_instructions_t* instructions);
static int  PutInteger(int value, compiler_instructions_t* instructions);

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

    *input_buffer = (char*) calloc(char_number + 2, sizeof(uint8_t));//1 for \0 another one fo adding \n
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

    fwrite(&COMPILER_VERSION , sizeof(size_t), 1, compiled_file);
    fwrite(&instructions->instructions_bytes_written , sizeof(size_t), 1, compiled_file);
    fwrite(instructions->instructions_array , sizeof(uint8_t), instructions->instructions_bytes_written, compiled_file);
    fprintf(compiled_file, "\n\nHere was pr1usf0x.\n");

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
    instructions->instructions_label_tabular = InitialiseLabelTabular();

    while (*(input_command) != '\0')
    {
        while (CheckIfLabel(input_command = SkipSpaces(input_command)))
        {
            *(SkipNotSpaces(input_command) - 1) = '\0';

            if (InitLabel(input_command, instructions) != LABEL_INSTRUCTION_RETURN_SUCCESS)
            {
                printf("EMPTY LABEL OR REUSED TWICE.\n");
                return COMPILER_RETURN_LABEL_ERROR;
            }

            input_command = SkipSpaces(SkipNotSpaces(input_command) + 1); //for skipping \0
        }

        input_command = SkipSpaces(input_command);

        compiler_return_e output = ReadCommand(&input_command, instructions);

        input_command++;
        command_index++;

        if (output == COMPILER_RETURN_INCORRECT_COMMAND)
        {
            printf("INCORRECT COMMAND IN LINE %zu.\n", command_index);
            return COMPILER_RETURN_INCORRECT_COMMAND;
        }
        if (output == COMPILER_RETURN_INVALID_SYNTAX)
        {
            printf("INCORRECT SYNTAX IN LINE %zu.\n", command_index);
            return COMPILER_RETURN_INVALID_SYNTAX;
        }
    }

    return COMPILER_RETURN_SUCCESS;
}

compiler_return_e
ReadCommand(char**                   input_command,
            compiler_instructions_t* instructions)
{
    size_t index_in_table = 0;

    size_t command_size = (size_t) (SkipNotSpaces(*input_command) - *input_command);

    if (command_size == 0)
    {
        return COMPILER_RETURN_EMPTY_COMMAND;
    }

    bool find_flag = false;
    for (size_t index = 0; index < COMMANDS_COUNT; index++)
    {
        if ((COMPILER_COMMANDS_ARRAY[index]).command_name == NULL)
        {
            continue;
        }
        if (strncmp(*input_command, COMPILER_COMMANDS_ARRAY[index].command_name, command_size) == 0)
        {
            if (strlen(COMPILER_COMMANDS_ARRAY[index].command_name) != command_size)
            {
                continue;
            }

            index_in_table = index;

            if (PutInstruction(index_in_table, instructions) != 0)
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

    if (COMPILER_COMMANDS_ARRAY[index_in_table].handler != NULL)
    {
        *input_command += command_size;
        *input_command = SkipSpaces(*input_command);

        compiler_return_e output = (COMPILER_COMMANDS_ARRAY[index_in_table].handler)(input_command, instructions);
        if (output != COMPILER_RETURN_VALID_SYNTAX)
        {
            return output;
        }
    }
    else
    {
        *input_command += command_size;
        *input_command = SkipSpaces(*input_command);
        if (**input_command != '\n')
        {
            return COMPILER_RETURN_INVALID_SYNTAX;
        }
    }

    return COMPILER_RETURN_SUCCESS;
}


// ================ HANDLERS ====================

static compiler_return_e
CheckIfMemoryOffset(char** input_command,
                    int*   read_value)
{
    ASSERT(input_command != NULL);
    ASSERT(read_value != NULL);

    int offset_value = 0;
    if (**input_command == '+')
    {
        offset_value = 1;
    }
    else if (**input_command == '-')
    {
        offset_value = -1;
    }
    else
    {
        return COMPILER_RETURN_VALID_SYNTAX;
    }

    *input_command = SkipSpaces(*input_command + 1);

    char* end_str = NULL;
    offset_value *= (int) strtol(*input_command, &end_str, 0);
    *input_command = SkipSpaces(end_str);

    *read_value = offset_value;

    return COMPILER_RETURN_VALID_SYNTAX;
}

compiler_return_e
ReadPushArgument(char**                   input_command,
                 compiler_instructions_t* instructions)
{
    ASSERT(*input_command != NULL);
    ASSERT(instructions != NULL);

    if (!isdigit(**input_command))
    {
        bool find_flag = false;
        bool memory_usage = false;

        if (**input_command == '[')
        {
            *input_command = SkipSpaces(*input_command + 1);
            memory_usage = true;
        }

        for (int register_number = 0; register_number < PROCESSOR_REG_COUNT; register_number++)
        {
            if (strncmp(PROCESSORS_REG[register_number], *input_command, strlen(PROCESSORS_REG[register_number])) == 0)
            {
                *input_command = SkipSpaces(*input_command + strlen(PROCESSORS_REG[register_number]));

                int offset = 0;
                if (memory_usage)
                {
                    CheckIfMemoryOffset(input_command, &offset);

                    if (**input_command != ']')
                    {
                        return COMPILER_RETURN_INVALID_SYNTAX;
                    }

                    (instructions->instructions_array)[instructions->instructions_bytes_written - 1] = (instructions->instructions_array)[instructions->instructions_bytes_written - 1] | USES_RAM;
                    (*input_command)++;
                }

                (instructions->instructions_array)[instructions->instructions_bytes_written - sizeof(uint8_t)] = (instructions->instructions_array)[instructions->instructions_bytes_written - sizeof(uint8_t)] | (uint8_t) register_number;

                if (offset != 0)
                {
                    (instructions->instructions_array)[instructions->instructions_bytes_written - sizeof(uint8_t)] = (instructions->instructions_array)[instructions->instructions_bytes_written - sizeof(uint8_t)] | USES_INT;
                    if ((PutInteger(offset, instructions) != 0))
                    {
                        return COMPILER_RETURN_INVALID_SYNTAX;
                    }
                }

                find_flag = true;

                break;
            }
        }
        if (!find_flag)
        {
            return COMPILER_RETURN_INVALID_SYNTAX;
        }
    }
    else
    {
        char* end_str = NULL;

        (instructions->instructions_array)[instructions->instructions_bytes_written - 1] = (instructions->instructions_array)[instructions->instructions_bytes_written - 1] | USES_INT;

        if ((PutInteger((int) strtol(*input_command, &end_str, 0), instructions) != 0) || (end_str - *input_command == 0))
        {
            return COMPILER_RETURN_INVALID_SYNTAX;
        }

        *input_command = end_str;
    }

    *input_command = SkipSpaces(*input_command);

    if (**input_command != '\n')
    {
        return COMPILER_RETURN_INVALID_SYNTAX;
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
    bool memory_usage = false;

    if (**input_command == '[')
    {
        *input_command = SkipSpaces(*input_command + 1);
        memory_usage = true;
    }

    for (int register_number = 0; register_number < PROCESSOR_REG_COUNT; register_number++)
    {
        if (strncmp(PROCESSORS_REG[register_number], *input_command, strlen(PROCESSORS_REG[register_number])) == 0)
        {
            *input_command = SkipSpaces(*input_command + strlen(PROCESSORS_REG[register_number]));

            int offset = 0;
            if (memory_usage)
            {
                CheckIfMemoryOffset(input_command, &offset);

                if (**input_command != ']')
                {
                    return COMPILER_RETURN_INVALID_SYNTAX;
                }

                (instructions->instructions_array)[instructions->instructions_bytes_written - 1] = (instructions->instructions_array)[instructions->instructions_bytes_written - 1] | USES_RAM;
                (*input_command)++;
            }

            (instructions->instructions_array)[instructions->instructions_bytes_written - 1] = (instructions->instructions_array)[instructions->instructions_bytes_written - 1] | (uint8_t) register_number;

            if (offset != 0)
            {
                (instructions->instructions_array)[instructions->instructions_bytes_written - sizeof(uint8_t)] = (instructions->instructions_array)[instructions->instructions_bytes_written - sizeof(uint8_t)] | USES_INT;
                if ((PutInteger(offset, instructions) != 0))
                {
                    return COMPILER_RETURN_INVALID_SYNTAX;
                }
            }

            find_flag = true;

            break;
        }
    }

    if (!find_flag)
    {
        return COMPILER_RETURN_INVALID_SYNTAX;
    }

    *input_command = SkipSpaces(*input_command);

    if (**input_command != '\n')
    {
        return COMPILER_RETURN_INVALID_SYNTAX;
    }

    return COMPILER_RETURN_VALID_SYNTAX;
}

compiler_return_e
ReadJumpArgument(char**                    input_command,
                 compiler_instructions_t*  instructions)
{
    ASSERT(input_command != NULL);
    ASSERT(instructions != NULL);

    if (**input_command == '\n')
    {
        return COMPILER_RETURN_INVALID_SYNTAX;
    }

    if (!CheckIfLabel(*input_command))
    {
        if (isdigit(**input_command))
        {
            char* end_str = NULL;

            if ((PutInteger((int) strtol(*input_command, &end_str, 0), instructions) != 0) || (end_str - *input_command == 0))
            {
                return COMPILER_RETURN_INVALID_SYNTAX;
            }
            *input_command = end_str;
        }
        else
        {
            return COMPILER_RETURN_INVALID_SYNTAX;
        }
    }
    else
    {
        if (*(SkipNotSpaces(*input_command) - 1) != ':')
        {
            return COMPILER_RETURN_INVALID_SYNTAX;
        }

        *(SkipNotSpaces(*input_command) - 1) = '\0';

        if (UseLabel(*input_command, instructions) != 0)
        {
            return COMPILER_RETURN_INVALID_SYNTAX;
        }

        instructions->instructions_bytes_written += sizeof(int);

        *input_command = SkipNotSpaces(*input_command) + 1;
    }

    *input_command = SkipSpaces(*input_command);
    if (**input_command != '\n')
    {
        return COMPILER_RETURN_INVALID_SYNTAX;
    }

    return COMPILER_RETURN_VALID_SYNTAX;
}

void
FreeAll(compiler_instructions_t* instructions,
        char*                    input_buffer)
{
    free(instructions->instructions_array);
    free(input_buffer);
    DestroyLabelTabular(instructions->instructions_label_tabular);

    memset(instructions, 0, sizeof(compiler_instructions_t));
}

compiler_return_e
ReadCallArgument(char**                    input_command,
                 compiler_instructions_t*  instructions)
{
    ASSERT(input_command != NULL);
    ASSERT(instructions != NULL);

    if (**input_command == '\n')
    {
        return COMPILER_RETURN_INVALID_SYNTAX;
    }

    *(SkipNotSpaces(*input_command)) = '\0';

    if (UseLabel(*input_command, instructions) != 0)
    {
        return COMPILER_RETURN_INVALID_SYNTAX;
    }

    *input_command = SkipNotSpaces(*input_command);

    instructions->instructions_bytes_written += sizeof(int);

    return COMPILER_RETURN_VALID_SYNTAX;
}

//================WORK_WITH_INSTR_ARRAY============

static int
PutInstruction(size_t                   index_in_table,
               compiler_instructions_t* instructions)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions->instructions_array != NULL);

    if (instructions->instructions_bytes_written >= instructions->instructions_max_bytes_amount - 5)
    {
        instructions->instructions_array = (uint8_t*) recalloc(instructions->instructions_array, instructions->instructions_max_bytes_amount, instructions->instructions_max_bytes_amount * 2);
        instructions->instructions_max_bytes_amount *= 2;
    }

    if (instructions->instructions_array == NULL)
    {
        return 1;
    }

    (instructions->instructions_array)[instructions->instructions_bytes_written] = COMPILER_COMMANDS_ARRAY[index_in_table].binary_value_block_1;

    if (!((COMPILER_COMMANDS_ARRAY[index_in_table].binary_value_block_1 & EXTENDED_PACK) ^ EXTENDED_PACK))
    {
        instructions->instructions_bytes_written += sizeof(uint8_t);
        (instructions->instructions_array)[instructions->instructions_bytes_written] = COMPILER_COMMANDS_ARRAY[index_in_table].binary_value_block_2;
    }

    instructions->instructions_bytes_written += sizeof(uint8_t);

    return 0;
}

static int
PutInteger(int                      value,
           compiler_instructions_t* instructions)
{
    ASSERT(instructions != NULL);
    ASSERT(instructions->instructions_array != NULL);

    if (instructions->instructions_bytes_written >= instructions->instructions_max_bytes_amount - 5)
    {
        instructions->instructions_array = (uint8_t*) recalloc(instructions->instructions_array, instructions->instructions_max_bytes_amount, instructions->instructions_max_bytes_amount * 2);
        instructions->instructions_max_bytes_amount *= 2;
    }

    if (instructions->instructions_array == NULL)
    {
        return 1;
    }

    memcpy(instructions->instructions_array + instructions->instructions_bytes_written, &value, sizeof(int));
    instructions->instructions_bytes_written += sizeof(int);

    return 0;
}

