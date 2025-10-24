#include <stdio.h>

#include "stack.h"
#include "processor_functions.h"
#include "simple_parser.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */

const char* ASSEMBLED_FILE_NAME = "compiled.obj";

int
main(int                argc,
     const char* const* argv)
{
    int output = 0;
    spu_t processor = {};

    if ((output = ReadFlags(argc, argv, &ASSEMBLED_FILE_NAME, &ASSEMBLED_FILE_NAME)) != 0)
    {
        return output;
    }

    if ((output = InitializeSPU(&processor, ASSEMBLED_FILE_NAME)) != 0)
    {
        DestroySPU(&processor);
        return output;
    };

    if ((output = ExecuteInstructions(&processor)) != 0)
    {
        DestroySPU(&processor);
        return output;
    };

    DestroySPU(&processor);

    return 0;
}


