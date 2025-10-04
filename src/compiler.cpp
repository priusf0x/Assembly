#include <stdio.h>

#include "commands.h"
#include "read_commands.h"
#include "color.h"
#include "tools.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */

const char* INPUT_FILE_NAME = "input_file.asm";

int main(void)
{
    char* input_buffer = NULL;
    size_t str_count = 0;
    string_t* array_of_strings = NULL;
    // int* instructions = calloc

    if (ReadFile(&input_buffer, &array_of_strings, &str_count, INPUT_FILE_NAME) != 0)
    {
        printf("Какая то хуйня\n");
    }

    for(size_t index = 0; index < str_count; index++)
    {
        commands_e output = ReadCommand(array_of_strings[index].string);

        if ((output == COMMAND_INCORRECT_COMMAND) || (output == COMMAND_INVALID_SYNTAX))
        {
            break;
        }
    }

    free(input_buffer);
    free(array_of_strings);

    return 0;
}
