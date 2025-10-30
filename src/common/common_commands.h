#ifndef COMMON_COMMANDS_H
#define COMMON_COMMANDS_H

#include <stdio.h>
#include <stdint.h>

const uint8_t EXTENDED_COMMAND_PACK = 0b00111000;
const uint8_t ARGUMENT_SWITCH_MASK = 0b11000000;
const uint8_t NEXT_BYTE_MASK = 0b11111000;
const uint8_t EXTENDED_ARGUMENT_MASK = 0b00111111;
const uint8_t ARGUMENT_MASK = 0b00000111;

//push/pops flags
const uint8_t USES_RAM = 0b00100000;
const uint8_t USES_INT = 0b00010000;
const uint8_t ADD_TO_REGI = 0b00001000;
const uint8_t REGISTER_MASK = 0b00000111;

enum commands_e
{
    COMMAND_HLT       = 0,
    COMMAND_PUSH      = 1,
    COMMAND_POP       = 2,
    COMMAND_OUT       = 3,
    COMMAND_OPERATION = 4,
    COMMAND_JMP       = 5,
    COMMAND_CALL      = 6,
    COMMAND_EMPTY_1   = 7,
    COMMAND_EMPTY_2   = 8,
    COMMAND_EMPTY_3   = 9,
    COMMAND_DRAW      = 10,

};

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
