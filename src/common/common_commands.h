#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

enum commands_e
{
    COMMAND_HLT                 = 0,
    COMMAND_PUSH                = 1,
    COMMAND_OUT                 = 2,
    COMMAND_ADD                 = 3,
    COMMAND_SUB                 = 4,
    COMMAND_MUL                 = 5,
    COMMAND_DIV                 = 6,
    COMMAND_PUSH_FROM_REG       = 7,
    COMMAND_POP                 = 8,
    COMMAND_SQRT                = 9,
    COMMAND_IN                  = 10,
    COMMAND_JMP                 = 11,
    COMMAND_JA                  = 12,
    COMMAND_JAE                 = 13,
    COMMAND_JB                  = 14,
    COMMAND_JBE                 = 15,
    COMMAND_JE                  = 16,
    COMMAND_JNE                 = 17,
    COMMAND_CALL                = 18,
    COMMAND_RET                 = 19,
    COMMAND_PUSH_FROM_MEMORY    = 20,
    COMMAND_POP_TO_MEMORY       = 21,
    COMMAND_DRAW                = 22,
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
