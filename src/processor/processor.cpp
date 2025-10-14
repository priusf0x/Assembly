#include <stdio.h>

#include "stack.h"
#include "processor_functions.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */
enum compiler_main_return_e
{
    PROCESSOR_MAIN_RETURN_SUCCESS,
    PROCESSOR_MAIN_RETURN_INIT_ERROR,
    PROCESSOR_MAIN_RETURN_EXECUTION_ERROR
};

int main(void)
{
    spu_t processor = {};

    if (InitializeSPU(&processor) != (int) PROCESSOR_MAIN_RETURN_SUCCESS)
    {
        return PROCESSOR_MAIN_RETURN_INIT_ERROR;
    };

    if (ExecuteInstructions(&processor) != (int) PROCESSOR_MAIN_RETURN_SUCCESS)
    {
        return PROCESSOR_MAIN_RETURN_EXECUTION_ERROR;
    };

    DestroySPU(&processor);

    return PROCESSOR_MAIN_RETURN_SUCCESS;
}


