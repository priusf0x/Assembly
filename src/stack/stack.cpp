#include "stack.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Assert.h"
#include "logger.h"
#include "tools.h"

const size_t CANARY_SIZE = 4;
const uint64_t CANARY_FILL = 0xB16B00B5;

static bool                    CheckCanary(stack_t* swag);
static stack_function_errors_e SetCanary(void* pointer, uint64_t value);
static stack_function_errors_e StackNormalizeSize(stack_t* swag);
static stack_function_errors_e VerifyStack(stack_t* swag);

struct stack_t
{
    const char* name;
    uint8_t* canary_start;
    uint8_t* canary_end;
    value_type* stack_data;
    size_t size;
    size_t capacity;
    size_t minimal_capacity;
    size_t real_capacity_in_bytes;
    enum stack_state_e state;
};

stack_function_errors_e
StackInit(stack_t**   swag,
          size_t      expected_capacity,
          const char* swag_name)
{
    ASSERT(swag_name != NULL);

    (*swag) = (stack_t*) calloc(1, sizeof(stack_t));

    (*swag)->name = swag_name;

    if (expected_capacity == 0)
    {
        return STACK_FUNCTION_INCORRECT_VALUE_ERROR;
    }

    (*swag)->real_capacity_in_bytes = sizeof(value_type) * expected_capacity + sizeof(long long) * (2 * CANARY_SIZE + 1);
    (*swag)->canary_start = (uint8_t*) calloc((*swag)->real_capacity_in_bytes, sizeof(uint64_t));
    if ((*swag)->canary_start == NULL)
    {
        (*swag)->state = STACK_STATE_MEMORY_ERROR;
        return STACK_FUNCTION_MEMORY_ERROR;
    }

    SetCanary((*swag)->canary_start, CANARY_FILL);

    (*swag)->stack_data = (value_type*) ((*swag)->canary_start + sizeof(uint64_t) * CANARY_SIZE);
    (*swag)->capacity = expected_capacity;
    (*swag)->minimal_capacity = expected_capacity;

    (*swag)->canary_end = (uint8_t*) ((*swag)->stack_data + expected_capacity);
    while ((size_t) (*swag)->canary_end % 8 != 0)
    {
        ((*swag)->canary_end)++;
    }
    SetCanary((*swag)->canary_end, CANARY_FILL);

    (*swag)->state = STACK_STATE_OK;

    return STACK_FUNCTION_SUCCESS;
}

stack_function_errors_e
StackDestroy(stack_t* swag)
{
    free(swag->canary_start);
    free(swag);

    return STACK_FUNCTION_SUCCESS;
}

stack_function_errors_e
StackPush(stack_t*   swag,
          value_type value)
{
    ASSERT(swag != NULL);

    VERIFY_STACK_WITH_RETURN(swag);

    StackNormalizeSize(swag);

    VERIFY_STACK_WITH_RETURN(swag);

    (swag->stack_data)[swag->size] = value;
    (swag->size)++;

    return STACK_FUNCTION_SUCCESS;
}

stack_function_errors_e
StackPop(stack_t*    swag,
         value_type* pop_variable)
{
    ASSERT(swag != NULL);

    VERIFY_STACK_WITH_RETURN(swag);

    StackNormalizeSize(swag);

    if ((swag->size) == 0)
    {
        *pop_variable = 0;
        return STACK_FUNCTION_EMPTY_STACK_ERROR;
    }

    (swag->size)--;
    *pop_variable = (swag->stack_data)[swag->size];
    (swag->stack_data)[swag->size] = 0;

    return STACK_FUNCTION_SUCCESS;
}

static bool
CheckCanary(stack_t* swag)
{
    for (size_t index = 0; index < CANARY_SIZE; index++)
    {
        if ((((uint64_t*) swag->canary_start)[index] != CANARY_FILL) || (((uint64_t*) swag->canary_end)[index] != CANARY_FILL))
        {
            return false;
        }
    }

    return true;
}

static stack_function_errors_e
VerifyStack(stack_t* swag)
{
    if ((swag->state) == STACK_STATE_UNINITIALIZED)
    {
        return STACK_FUNCTION_UNINITIALIZED_ERROR;
    }
    else if (((swag->state) == STACK_STATE_ZERO_CAPACITY) || (swag->capacity) == 0)
    {
        (swag->state) = STACK_STATE_ZERO_CAPACITY;
        return STACK_FUNCTION_ZERO_CAPACITY;
    }
    else if ((swag->stack_data) == NULL)
    {
        return STACK_FUNCTION_NULL_POINTER_ERROR;
    }
    else if (!CheckCanary(swag))
    {
        StackDump(swag);
        return STACK_FUNCTION_MEMORY_ERROR;
    }
    return STACK_FUNCTION_SUCCESS;
}


static stack_function_errors_e
StackNormalizeSize(stack_t* swag)
{
    if (swag->size == swag->capacity)
    {
        SetCanary(swag->canary_end, 0);
        (swag->canary_start) = (uint8_t*) recalloc(swag->canary_start, swag->real_capacity_in_bytes, swag->real_capacity_in_bytes + sizeof(value_type) * swag->capacity);

        swag->real_capacity_in_bytes += sizeof(value_type) * swag->capacity;

        swag->stack_data = (value_type*) (swag->canary_start + sizeof(uint64_t) * CANARY_SIZE);
        swag->capacity *= 2;

        swag->canary_end = (uint8_t*) (swag->stack_data + swag->capacity);
        while ((size_t) swag->canary_end % 8 != 0)
        {
            (swag->canary_end)++;
        }
        SetCanary(swag->canary_end, CANARY_FILL);
    }
    else if((2 * swag->size < swag->capacity) && (swag->capacity > 2 * swag->minimal_capacity))
    {
        SetCanary(swag->canary_end, 0);

        (swag->canary_start) = (uint8_t*) realloc(swag->canary_start, swag->real_capacity_in_bytes - sizeof(value_type) * ((swag->capacity) / 2));

        swag->real_capacity_in_bytes -= sizeof(value_type) * swag->capacity / 2;

        swag->stack_data = (value_type*) (swag->canary_start + sizeof(uint64_t) * CANARY_SIZE);
        swag->capacity -= swag->capacity / 2;

        swag->canary_end = (uint8_t*) (swag->stack_data + swag->capacity);
        while  ((size_t) swag->canary_end % 8 != 0)
        {
            (swag->canary_end)++;
        }
        SetCanary(swag->canary_end, CANARY_FILL);
    }

    return STACK_FUNCTION_SUCCESS;
}

static stack_function_errors_e
SetCanary(void*    pointer,
          uint64_t value)
{
    for (size_t index = 0; index < CANARY_SIZE; index++)
    {
        ((uint64_t*)pointer)[index] = value;
    }

    return STACK_FUNCTION_SUCCESS;
}

log_function_return_value_e
StackDump(stack_t* swag)
{
    FILE * log_file = GetLogFile();

    if ((swag == NULL) || (log_file == NULL))
    {
        return LOG_FUNCTION_NULL_POINTER_ERROR;
    }

    if (DETALIZATION_LEVEL >= DETALIZATION_LEVEL_DEBUG)
    {
        time_t t1 = time(NULL);
        tm t = *localtime(&t1);
        fprintf(log_file, "\n <%.2d:%.2d:%.2d> \n", t.tm_hour, t.tm_min, t.tm_sec);
        fprintf(log_file, "name = %s.\n", swag->name);
        fprintf(log_file, "size = %zu.\n", swag->size);
        fprintf(log_file, "capacity = %zu.\n", swag->capacity);
        fprintf(log_file, "real_capacity = %zu bytes.\n", swag->real_capacity_in_bytes);
        fprintf(log_file, "canary_start = %p.\n", swag->canary_start);
        fprintf(log_file, "stack_data = %p.\n", swag->stack_data);
        fprintf(log_file, "canary_end = %p.\n", swag->canary_end);

        switch(swag->state)
        {
        case STACK_STATE_OK:
            fprintf(log_file, "state = OK.\n");
            break;

        case STACK_STATE_UNINITIALIZED:
            fprintf(log_file, "state = UNINITIALIZED.\n");
            break;

        case STACK_STATE_MEMORY_ERROR:
            fprintf(log_file, "state = MEMORY_ERROR.\n");
            break;

        case STACK_STATE_ZERO_CAPACITY:
            fprintf(log_file, "state = ZERO_CAPACITY.\n");
            break;

        default: break;
        }

        fprintf(log_file, "----------DATA----------\n");

        for (size_t index = 0; index < swag->real_capacity_in_bytes; index++)
        {
            if (index % 8 == 0)
            {
                fprintf(log_file, "\n");
            }

            fprintf(log_file, "[%3.0zu]%4d  ", (size_t) &((swag->canary_start)[index]) % 1000,(swag->canary_start)[index]);

        }
    }
    return LOG_FUNCTION_SUCCESS;
}


