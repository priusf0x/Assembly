#include <stdio.h>

#include "color.h"
#include "common_commands.h"
#include "tools.h"
#include "read_commands.h"
#include "simple_parser.h"


/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */

// Adding features
//  -Readme(REQUIRED) (2 priority)
//  -add processor_verifier (2 priority)

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

int
main(int                argc,
     const char* const* argv)
{
    char* input_buffer = NULL;
    compiler_instructions_t instructions = {.instructions_count = 0, .instructions_size = 3, .instructions_array = NULL};
    instructions.instructions_array = (int*) calloc(instructions.instructions_size, sizeof(int));

    if (ReadFlags(argc, argv, &INPUT_FILE_NAME, &COMPILED_NAME) != (int) COMPILER_MAIN_RETURN_SUCCESS)
    {
        free(instructions.instructions_array);
        printf(RED "FLAG READ ERROR.\n" STANDARD);
        return COMPILER_MAIN_RETURN_FILE_READ_ERROR;
    }

    if (ReadFile(&input_buffer, INPUT_FILE_NAME) != (int) COMPILER_MAIN_RETURN_SUCCESS)
    {
        FreeAll(&instructions, input_buffer);
        printf(RED "FILE OPEN ERROR.\n" STANDARD);
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
        printf(RED "FILE WRITE ERROR.\n" STANDARD);
        return COMPILER_MAIN_RETURN_FILE_CLOSE_ERROR;
    }

    FreeAll(&instructions, input_buffer);

    return COMPILER_MAIN_RETURN_SUCCESS;
}



