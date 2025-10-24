#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

#include <stdio.h>
#include <stdint.h>

const uint8_t EXTENDED_PACK = 0b11000000;
const uint8_t ARGUMENT_MASK = 0b00111111;

//push/pops flags
const uint8_t USES_RAM = 0b00100000;
const uint8_t USES_INT = 0b00010000;
const uint8_t ADD_TO_REGI = 0b00001000;
const uint8_t REGISTER_MASK = 0b00000111;

enum commands_e
{
    COMMAND_HLT                 = 0, //  |00| 000000
    COMMAND_PUSH                = 1, //  |01| 00REGI
    COMMAND_POP                 = 2, //  |10| 00REGI
    COMMAND_OUT                 = 3, //  |11| 000001 00|000000
    COMMAND_OPERATION           = 4, //  |11| 000010 00|000000
    COMMAND_JMP                 = 5, //  |11| 000011 00|000000
    COMMAND_CALL                = 6, //  |11| 000100 00|000000
    COMMAND_DRAW                = 7  //  |00| 000101 00|000000
};

//example of my independent command coding
//1 priority 01|ARGUME (Count 3)
//2 priority 11|010001 01|ARGUME (Count 64*3)
//3 priority 11|011101 11|011100 10|ARGUME (Count ??)
// ...

const char* const PROCESSORS_REG[] =
{
    "R0X",
    "RAX",
    "RBX",
    "RCX",
    "RDX",
    "REX",
    "RFX",
    "RGX"
};
const int PROCESSOR_REG_COUNT = sizeof(PROCESSORS_REG) /  sizeof(PROCESSORS_REG[0]);

#endif //COMMON_COMMANDS_H
