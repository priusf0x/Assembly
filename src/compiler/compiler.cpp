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
//  -add processor_verifier (2 priority)
// -clear shit from code

const char* INPUT_FILE_NAME = "input_file.asm";
const char* COMPILED_NAME = "compiled.obj";

int
main(int                argc,
     const char* const* argv)
{
    char* input_buffer = NULL;
    int output = 0;
    compiler_instructions_t instructions = {.instructions_bytes_written = 0, .instructions_max_bytes_amount = 20, .instructions_array = NULL};
    instructions.instructions_array = (uint8_t*) calloc(instructions.instructions_max_bytes_amount, sizeof(uint8_t));

    if ((output = ReadFlags(argc, argv, &INPUT_FILE_NAME, &COMPILED_NAME)) != 0)
    {
        free(instructions.instructions_array);
        printf(RED "FLAG READ ERROR.\n" STANDARD);
        return output;
    }

    if ((output = ReadFile(&input_buffer, INPUT_FILE_NAME)) != 0)
    {
        FreeAll(&instructions, input_buffer);
        printf(RED "FILE OPEN ERROR.\n" STANDARD);
        return output;
    }

    if ((output = TranslateCode(input_buffer, &instructions) != 0))
    {
        FreeAll(&instructions, input_buffer);
        return output;
    }

    if ((output = FixUp(&instructions)) != 0)
    {
        LabelTabularDump(&instructions);
        printf(RED "FIXUP ERROR" STANDARD);
        FreeAll(&instructions, input_buffer);
        return output;
    }


    if ((output = WriteInFile(&instructions, COMPILED_NAME)) != 0)
    {
        FreeAll(&instructions, input_buffer);
        printf(RED "FILE WRITE ERROR.\n" STANDARD);
        return output;
    }

    FreeAll(&instructions, input_buffer);
    return 0;
}



