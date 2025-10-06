#include <stdio.h>

#include "tools.h"
#include "stack.h"
#include "processor_functions.h"

/*Dear programmer:
 *When I wrote this code, only god and
 *I knew how it worked.
 *Now, only god knows it!
 */

int main(void)
{
    spu_t processor = {};

    InitializeSPU(&processor);

    ExecuteInstructions(&processor);

    DestroySPU(&processor);

    return 0;
}


