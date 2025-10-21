#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

#define USES_MEMORY      0b00001000
#define USES_EXTRA_SPACE 0b10000000
#define COMMAND_MASK     0b01110000
#define REGISTER_MASK    0b00000111
#define ARGUMENT_MASK    0b00001111

enum commands_e
{
    COMMAND_HLT                 = 0, //0   |000| 0000
    COMMAND_PUSH                = 1, //0(1)|001| 0000 (MEMORY_BYTE-REG_3_BYTES)
    COMMAND_OUT                 = 2, //0   |010| 0000
    COMMAND_OPERATION           = 3, //0   |011| 0000
    COMMAND_POP                 = 4, //0   |100| 0000 (MEMORY_BYTE-REG_3_BYTES)
    COMMAND_JMP                 = 5, //0   |101| 0000
    COMMAND_CALL                = 6, //0   |110| 0000
    COMMAND_DRAW                = 7, //0   |111| 0000
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
