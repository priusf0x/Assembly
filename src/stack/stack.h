#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdint.h>

#include "tools.h"
#include "logger.h"

typedef int value_type;

enum stack_function_errors_e
{
    STACK_FUNCTION_SUCCESS               = 0,
    STACK_FUNCTION_INITIALIZATION_ERROR  = 1,
    STACK_FUNCTION_UNINITIALIZED_ERROR   = 2,
    STACK_FUNCTION_NULL_POINTER_ERROR    = 3,
    STACK_FUNCTION_MEMORY_ERROR          = 4,
    STACK_FUNCTION_ZERO_CAPACITY         = 5,
    STACK_FUNCTION_INCORRECT_VALUE_ERROR = 6,
    STACK_FUNCTION_EMPTY_STACK_ERROR     = 7,
    STACK_FUNCTION_NOT_ENOUGH_ELEMENTS   = 8
};

enum stack_state_e
{
    STACK_STATE_UNINITIALIZED = 0,
    STACK_STATE_OK            = 1,
    STACK_STATE_MEMORY_ERROR  = 2,
    STACK_STATE_ZERO_CAPACITY = 3
};

struct stack_t;

stack_function_errors_e     StackInit(stack_t** swag, size_t count, const char* swag_name);
stack_function_errors_e     StackDestroy(stack_t* swag);
stack_function_errors_e     StackPush(stack_t* swag, value_type value);
stack_function_errors_e     StackPop(stack_t* swag, value_type* pop_variable);
log_function_return_value_e StackDumpInLog(stack_t* swag);
log_function_return_value_e StackDump(stack_t* swag);

#ifdef HARD_SWAG
#define VERIFY_STACK_WITH_RETURN(X)
#else
#define VERIFY_STACK_WITH_RETURN(X) if (VerifyStack(X) != STACK_FUNCTION_SUCCESS) {LOGSHIT(DETALIZATION_LEVEL_ERROR, "STACK_ERROR %d with %s was occupied", VerifyStack(X), X->name);return VerifyStack(X);}
#endif

#endif //STACK_H
