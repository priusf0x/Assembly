#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

enum commands_e
{
    COMMAND_HLT               = 0,
    COMMAND_PUSH              = 1,
    COMMAND_OUT               = 2,
    COMMAND_ADD               = 3,
    COMMAND_SUB               = 4,
    COMMAND_MUL               = 5,
    COMMAND_DIV               = 6,
    COMMAND_PUSH_IN_REG       = 7,
    COMMAND_POP               = 8,
    COMMAND_SQRT              = 9,
    COMMAND_IN                = 10
};

const char* const PROCESSORS_REG[] =
{
    "R0X",
    "RAX",
    "RBX",
    "RCX",
    "RDX"
};
const int PROCESSOR_REG_COUNT = sizeof(PROCESSORS_REG) /  sizeof(PROCESSORS_REG[0]);

#endif //COMMON_COMMANDS_H
