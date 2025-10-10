#ifndef LABELS_H
#define LABELS_H

#include <stdio.h>

#include "compiler_commands.h"

struct label_tabular_t;

enum label_instruction_return_e
{
    LABEL_INSTRUCTION_RETURN_SUCCESS,
    LABEL_INSTRUCTION_RETURN_MEMORY_ERROR,
    LABEL_INSTRUCTION_RETURN_USAGE_REPEAT,
    LABEL_INSTRUCTION_RETURN_INITIALIZATION_REPEAT,
    LABEL_INSTRUCTION_RETURN_EMPTY_NAME
};

bool                       CheckIfLabel(char* string);
label_tabular_t*           InitialiseLabelTabular();
label_instruction_return_e InitLabel(char* label_name, compiler_instructions_t* instructions, label_tabular_t* label_tabular);
label_instruction_return_e UseLabel(char* label_name, compiler_instructions_t* instructions, label_tabular_t* label_tabular);
void                       LabelTabularDump(label_tabular_t* label_tabular);
void                       DestroyLabelTabular(label_tabular_t* label_tabular);


#endif //LABELS_H
