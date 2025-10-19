#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

enum commands_e
{
    COMMAND_HLT                 = 0, //0   |0000|000
    COMMAND_PUSH                = 1, //0(1)|0001| 000(111)
    COMMAND_OUT                 = 2, //0   |0010| 000
    COMMAND_OPERATION           = 3, //0   |0011| 001(111)
    COMMAND_POP                 = 4, //0   |0100| 000(111)
    COMMAND_IN                  = 5, //0   |0101| 000(111)
    COMMAND_JMP                 = 6, //0   |0110| 000(111)
    COMMAND_CALL                = 7, //0   |0111| 000(001)
    COMMAND_DRAW                = 8, //0   |1000| 000(
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
