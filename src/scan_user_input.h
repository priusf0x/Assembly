#ifndef SCAN_USER_INPUT
#define SCAN_USER_INPUT

#include "stack.h"
#include "calculator.h"

bool ClearBuffer();
void ReadUserInput(stack_t* swag, calculator_commands_e*  input_command);
void ReadCoefficient(stack_t* swag, calculator_commands_e* input_command);
void CheckIfSpaces(calculator_commands_e* expected_state);

#endif //SCAN_USER_INPUT
