#include <stdio.h>

#include "color.h"
#include "common_commands.h"
#include "read_commands.h"
#include "tools.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */

// Adding features
//  -Adding strtol
//  -Adding File_Parser
//  -Rewrite makefile(add compilations flags,build all etc.)
//  -Readme(REQUIRED)

enum compiler_main_return_e
{
    COMPILER_MAIN_RETURN_SUCCESS,
    COMPILER_MAIN_RETURN_FILE_READ_ERROR,
    COMPILER_MAIN_RETURN_COMPILATION_ERROR,
    COMPILER_MAIN_RETURN_FIXUP_ERROR,
    COMPILER_MAIN_RETURN_FILE_CLOSE_ERROR
};

const char* INPUT_FILE_NAME = "input_file.asm";
const char* COMPILED_NAME = "compiled.obj";

int main(void)
{
    char* input_buffer = NULL;
    compiler_instructions_t instructions = {.instructions_count = 0, .instructions_size = 3, .instructions_array = NULL};
    instructions.instructions_array = (int*) calloc(instructions.instructions_size, sizeof(int));

    if (ReadFile(&input_buffer, INPUT_FILE_NAME) != (int) COMPILER_MAIN_RETURN_SUCCESS)
    {
        FreeAll(&instructions, input_buffer);
        printf("FILE OPEN ERROR.\n");
        return COMPILER_MAIN_RETURN_FILE_READ_ERROR;
    }

    if (TranslateCode(input_buffer, &instructions) != (int) COMPILER_MAIN_RETURN_SUCCESS)
    {
        FreeAll(&instructions, input_buffer);
        return COMPILER_MAIN_RETURN_COMPILATION_ERROR;
    }

    // LabelTabularDump(&instructions);

    if (FixUp(&instructions) != (int) COMPILER_MAIN_RETURN_SUCCESS)
    {
        FreeAll(&instructions, input_buffer);
        return COMPILER_MAIN_RETURN_FIXUP_ERROR;
    }

    if (WriteInFile(&instructions, COMPILED_NAME) != (int) COMPILER_MAIN_RETURN_SUCCESS)
    {
        FreeAll(&instructions, input_buffer);
        printf("FILE WRITE ERROR.\n");
        return COMPILER_MAIN_RETURN_FILE_CLOSE_ERROR;
    }

    FreeAll(&instructions, input_buffer);

    return COMPILER_MAIN_RETURN_SUCCESS;
}



