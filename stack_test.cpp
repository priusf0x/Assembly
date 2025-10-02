#include <stdio.h>

#include "stack.h"
#include "logger.h"


int main(void)
{
    stack_t swag = {};

    StackInit(&swag, 3, "meow_stack");

    swag.stack_data[0] = 1;

    StackPush(&swag, 2);
    StackPush(&swag, 4);
    StackPush(&swag, 5);
    StackPush(&swag, 2);
    StackPush(&swag, 4);
    StackPush(&swag, 5);

    value_type x = 0;
    StackDump(&swag);

    StackPop(&swag, &x);
    StackPop(&swag, &x);
    StackPop(&swag, &x);
    StackPop(&swag, &x);
    StackPop(&swag, &x);
    StackPop(&swag, &x);

    StackDump(&swag);

    StackDestroy(&swag);

    return 0;
}
