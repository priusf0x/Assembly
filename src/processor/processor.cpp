#include <stdio.h>

#include "stack.h"
#include "processor_functions.h"
#include "simple_parser.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */

enum
processor_main_return_e
{
    PROCESSOR_MAIN_RETURN_SUCCESS,
    PROCESSOR_MAIN_RETURN_FILE_READ_ERROR,
    PROCESSOR_MAIN_RETURN_INIT_ERROR,
    PROCESSOR_MAIN_RETURN_EXECUTION_ERROR
};

const char* ASSEMBLED_FILE_NAME = "compiled.obj";

int
main(int                argc,
     const char* const* argv)
{
    spu_t processor = {};

    if (ReadFlags(argc, argv, &ASSEMBLED_FILE_NAME, &ASSEMBLED_FILE_NAME) != (int) PROCESSOR_MAIN_RETURN_SUCCESS)
    {
        return PROCESSOR_MAIN_RETURN_FILE_READ_ERROR;
    }

    if (InitializeSPU(&processor, ASSEMBLED_FILE_NAME) != (int) PROCESSOR_MAIN_RETURN_SUCCESS)
    {
        DestroySPU(&processor);
        return PROCESSOR_MAIN_RETURN_INIT_ERROR;
    };

    if (ExecuteInstructions(&processor) != (int) PROCESSOR_MAIN_RETURN_SUCCESS)
    {
        DestroySPU(&processor);
        return PROCESSOR_MAIN_RETURN_EXECUTION_ERROR;
    };

    DestroySPU(&processor);

    return PROCESSOR_MAIN_RETURN_SUCCESS;
}


