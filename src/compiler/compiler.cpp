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
    compiler_instructions_t instructions = {.instructions_count = 0, .instructions_size = 3, .instructions_array = NULL};
    instructions.instructions_array = (int*) calloc(instructions.instructions_size, sizeof(int));

    if (ReadFile(&input_buffer, INPUT_FILE_NAME) != 0)
    {
        FreeAll(&instructions, input_buffer);
        printf("FILE OPEN ERROR.\n");
        return 1;
    }

    if (TranslateCode(input_buffer, &instructions) != COMPILER_RETURN_SUCCESS)
    {
        FreeAll(&instructions, input_buffer);
        return 1;
    }

    if (WriteInFile(&instructions, COMPILED_NAME) != READ_FILE_ERROR_TYPE_SUCCESS)
    {
        FreeAll(&instructions, input_buffer);
        return 1;
    }

    FreeAll(&instructions, input_buffer);

    return 0;
}



