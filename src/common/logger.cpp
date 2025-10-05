#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "stack.h"
#include "tools.h"


const size_t LOG_BUFFER_SIZE = 100;
enum detalization_levels_e DETALIZATION_LEVEL = DETALIZATION_LEVEL_DEBUG;
char LOG_BUFFER[LOG_BUFFER_SIZE] = {0};

FILE*
GetLogFile()
{
    static FILE * log_file = OpenLogFile();
    return log_file;
}

FILE*
OpenLogFile()
{
    FILE * log_file = fopen(LOG_FILE, "a+");
    if (log_file == NULL)
    {
        fprintf(stderr, "FAILED TO READ FILE\n");
        exit(EXIT_FAILURE);
        return NULL;
    }
    fprintf(log_file, "\n-------------------------------------\n");
    return log_file;
}

log_function_return_value_e
LogMessage(const char * log_message,
           enum detalization_levels_e detalization_level)
{
    FILE * log_file = GetLogFile();

    if ((log_message == NULL) || (log_file == NULL))
    {
        return LOG_FUNCTION_NULL_POINTER_ERROR;
    }

    if (detalization_level <= DETALIZATION_LEVEL)
    {
        time_t t1 = time(NULL);
        tm t = *localtime(&t1);
        fprintf(log_file, "\n <%.2d:%.2d:%.2d> ", t.tm_hour, t.tm_min, t.tm_sec);
        fprintf(log_file, "%s", log_message);
    }

    return LOG_FUNCTION_SUCCESS;
}

void
SwitchDetailLevelTo(enum detalization_levels_e detalization_level)
{
    DETALIZATION_LEVEL = detalization_level;
}

//
// log_function_return_value_e
// StackDump(stack_t* swag)
// {
//     FILE * log_file = GetLogFile();
//
//     if ((swag == NULL) || (log_file == NULL))
//     {
//         return LOG_FUNCTION_NULL_POINTER_ERROR;
//     }
//
//     if (DETALIZATION_LEVEL >= DETALIZATION_LEVEL_DEBUG)
//     {
//         time_t t1 = time(NULL);
//         tm t = *localtime(&t1);
//         fprintf(log_file, "\n <%.2d:%.2d:%.2d> \n", t.tm_hour, t.tm_min, t.tm_sec);
//         fprintf(log_file, "name = %s.\n", swag->name);
//         fprintf(log_file, "size = %zu.\n", swag->size);
//         fprintf(log_file, "capacity = %zu.\n", swag->capacity);
//
//         switch(swag->state)
//         {
//         case STACK_STATE_OK:
//             fprintf(log_file, "state = OK.\n");
//             break;
//
//         case STACK_STATE_UNINITIALIZED:
//             fprintf(log_file, "state = UNINITIALIZED.\n");
//             break;
//
//         case STACK_STATE_MEMORY_ERROR:
//             fprintf(log_file, "state = MEMORY_ERROR.\n");
//             break;
//
//         case STACK_STATE_ZERO_CAPACITY:
//             fprintf(log_file, "state = ZERO_CAPACITY.\n");
//             break;
//
//         default: break;
//         }
//
//         fprintf(log_file, "----------DATA----------\n");
//
//         for (size_t index = 0; index < CANARY_SIZE; index++)
//         {
//                 fprintf(log_file, "[CANARY] %d \n", *((uint8_t*) (swag->stack_data) - CANARY_SIZE + index));
//         }
//
//         for (size_t index = 0; index < swag->capacity; index++)
//
//             if (index < swag->size)
//             {
//                 fprintf(log_file, "[%zu ELEMENT] %d \n", index + 1, (((uint8_t*)swag->stack_data + CANARY_SIZE   )[index]);
//             }
//             else
//             {
//                 fprintf(log_file, "[%zu ELEMENT] nothing_here \n", index + 1);
//             }
//         }
//
//         for (size_t index = 0; index < CANARY_SIZE; index++)
//         {
//                 fprintf(log_file, "[CANARY] %d \n", *((uint8_t*) (swag->stack_data + swag->capacity) + CANARY_SIZE+ index));
//         }
//
//     return LOG_FUNCTION_SUCCESS;
// }







