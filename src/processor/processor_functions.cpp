#include "processor_functions.h"

#include <stdio.h>

#include "Assert.h"
#include "color.h"
#include "logger.h"
#include "stack.h"

int
InitializeSPU(spu_t* spu)
{

    return 0;
}
// stack_function_errors_e StartCalculator(stack_t* calculator_stack)
// {
//     ASSERT(calculator_stack);
//
//     VERIFY_STACK_WITH_RETURN(calculator_stack);
//
//     calculator_commands_e input_command = CALCULATOR_COMMAND_START;
//
//     while (input_command != CALCULATOR_COMMAND_HLT)
//     {
//         ((commands_array[input_command]).command_function)(calculator_stack, &input_command);
//     }
//
//     return STACK_FUNCTION_SUCCESS;
// }
//
// //================ COMMANDS_FUNCTIONS =========================
//
// void
// StackOut(stack_t*               calculator_stack,
//                   calculator_commands_e* input_command)
// {
//     ASSERT(calculator_stack != NULL);
//     ASSERT(input_command != NULL);
//
//     CheckIfSpaces(input_command);
//     if (*input_command == CALCULATOR_COMMAND_INVALID_SYNTAX)
//     {
//         return;
//     }
//
//     value_type intermediate_value = 0;
//     stack_function_errors_e return_error = StackPop(calculator_stack, &intermediate_value);
//
//     if (return_error != STACK_FUNCTION_SUCCESS)
//     {
//         LOG_FUNCTION_ERROR(return_error);
//         *input_command = CALCULATOR_COMMAND_PROGRAM_ERROR;
//         return;
//     }
//     printf("%d\n", intermediate_value);
//     *input_command = CALCULATOR_COMMAND_START;
// }
//
// stack_function_errors_e
// StackAdd(stack_t* swag)
// {
//     ASSERT(swag != NULL);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     value_type value_1 = 0;
//     value_type value_2 = 0;
//
//     if (swag->size < 2)
//     {
//         return STACK_FUNCTION_NOT_ENOUGH_ELEMENTS;
//     }
//
//     StackPop(swag, &value_1);
//     StackPop(swag, &value_2);
//
//     StackPush(swag, value_1 + value_2);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     return STACK_FUNCTION_SUCCESS;
// }
//
// stack_function_errors_e
// StackSub(stack_t* swag)
// {
//     ASSERT(swag != NULL);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     value_type value_1 = 0;
//     value_type value_2 = 0;
//
//     if (swag->size < 2)
//     {
//         return STACK_FUNCTION_NOT_ENOUGH_ELEMENTS;
//     }
//
//     StackPop(swag, &value_1);
//     StackPop(swag, &value_2);
//
//     StackPush(swag, value_2 - value_1);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     return STACK_FUNCTION_SUCCESS;
// }
//
// stack_function_errors_e
// StackMul(stack_t* swag)
// {
//     ASSERT(swag != NULL);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     value_type value_1 = 0;
//     value_type value_2 = 0;
//
//     if (swag->size < 2)
//     {
//         return STACK_FUNCTION_NOT_ENOUGH_ELEMENTS;
//     }
//
//     StackPop(swag, &value_1);
//     StackPop(swag, &value_2);
//
//     StackPush(swag, value_1 * value_2);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     return STACK_FUNCTION_SUCCESS;
// }
//
// stack_function_errors_e
// StackDiv(stack_t* swag)
// {
//     ASSERT(swag != NULL);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     value_type value_1 = 0;
//     value_type value_2 = 0;
//
//     if (swag->size < 2)
//     {
//         return STACK_FUNCTION_NOT_ENOUGH_ELEMENTS;
//     }
//
//     StackPop(swag, &value_1);
//     StackPop(swag, &value_2);
//
//     StackPush(swag, value_2 / value_1);
//
//     VERIFY_STACK_WITH_RETURN(swag);
//
//     return STACK_FUNCTION_SUCCESS;
// }
//
