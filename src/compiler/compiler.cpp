#include <stdio.h>

#include "common_commands.h"
#include "read_commands.h"
#include "color.h"
#include "tools.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */

const char* INPUT_FILE_NAME = "input_file.asm";
const char* COMPILED_NAME = "compiled.obj";

int main(void)
{
    char* input_buffer = NULL;
    size_t str_count = 0;
    string_t* array_of_strings = NULL;
    compiler_instructions_t instructions = {.instructions_count = 0, .instructions_size = 3, .instructions_array = NULL};
    instructions.instructions_array = (int*) calloc(instructions.instructions_size, sizeof(int));

    if (ReadFile(&input_buffer, &array_of_strings, &str_count, INPUT_FILE_NAME) != 0)
    {
        FreeAll(&instructions, input_buffer, array_of_strings);
        printf("FILE OPEN ERROR.\n");
        exit(EXIT_FAILURE);
    }

    for(size_t index = 0; index < str_count; index++)
    {
        compiler_return_e output = Readcommand(array_of_strings[index].string, &instructions);

        if (output == COMPILER_RETURN_INCORRECT_COMMAND)
        {
            FreeAll(&instructions, input_buffer, array_of_strings);
            printf("INCORRECT command IN LINE %zu.\n", index + 1);
            exit(EXIT_FAILURE);
        }
        if (output == COMPILER_RETURN_INVALID_SYNTAX)
        {
            FreeAll(&instructions, input_buffer, array_of_strings);
            printf("INCORRECT SYNTAX IN LINE %zu.\n", index + 1);
            exit(EXIT_FAILURE);
        }
    }

    instructions.instructions_array[0] = (int) instructions.instructions_count;

    FILE* compiled_file = fopen(COMPILED_NAME, "wb+");
    if (compiled_file == NULL)
    {
        FreeAll(&instructions, input_buffer, array_of_strings);
        printf("FILE WRITE ERROR.\n");
        exit(EXIT_FAILURE);
    }

    fwrite(instructions.instructions_array , sizeof(int), instructions.instructions_count + 1, compiled_file);
    fprintf(compiled_file, "\nHere was pr1usf0x.\n");

    if (fclose(compiled_file) != 0)
    {
        FreeAll(&instructions, input_buffer, array_of_strings);
        printf("FILE WRITE ERROR.\n");
        exit(EXIT_FAILURE);
    }

    FreeAll(&instructions, input_buffer, array_of_strings);

    return 0;
}

