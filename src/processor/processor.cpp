#include <stdio.h>

#include "stack.h"
#include "processor_functions.h"
#include "simple_parser.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */


int
main(int                argc,
     const char* const* argv)
{
    int error_code = 0;
    spu_t processor = {};

    const char* assembled_file_name = "compiled.obj";
    if ((error_code = ReadFlags(argc, argv, &assembled_file_name, &assembled_file_name)) != 0)
    {
        return error_code;
    }

    if ((error_code = InitializeSPU(&processor, assembled_file_name)) != 0)
    {
        DestroySPU(&processor);
        return error_code;
    };

    if ((error_code = ExecuteInstructions(&processor)) != 0)
    {
        DestroySPU(&processor);
        return error_code;
    };

    DestroySPU(&processor);

    return 0;
}


