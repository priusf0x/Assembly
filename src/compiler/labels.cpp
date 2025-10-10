#include "labels.h"

#include <stdlib.h>
#include <string.h>

#include "Assert.h"
#include "color.h"
#include "tools.h"
#include "compiler_commands.h"

const size_t TABULAR_START_SIZE = 5;
const float  INCREASE_COEFFICIENT = 2;

struct label_t
{
    char*  name;
    size_t from;
    size_t to;
    bool   is_initialized;
    bool   is_used;
};

struct  label_tabular_t
{
    label_t* labels;
    size_t   count;
    size_t   size;
};

bool
CheckIfLabel(char* string)
{
    ASSERT(string);

    char* intermidiate_string = SkipNotSpaces(string);

    if ((*(intermidiate_string - 1) == ':') && ((intermidiate_string - string) > 0))
    {
        return true;
    }

    return false;
}

label_tabular_t*
InitialiseLabelTabular()
{
    label_tabular_t* return_tabular = (label_tabular_t*) calloc(TABULAR_START_SIZE, sizeof(label_tabular_t));

    return_tabular->labels = (label_t*) calloc(TABULAR_START_SIZE, sizeof(label_t));
    return_tabular->size = TABULAR_START_SIZE;
    return_tabular->count = 0;

    return return_tabular;
};

label_instruction_return_e
UseLabel(char*                    label_name,
         compiler_instructions_t* instructions,
         label_tabular_t*         label_tabular)
{
    ASSERT(label_name != NULL);
    ASSERT(instructions != NULL);
    ASSERT(label_tabular != NULL);

    if (strlen(label_name) == 0)
    {
        return LABEL_INSTRUCTION_RETURN_EMPTY_NAME;
    }
    if (label_tabular->count == label_tabular->size)
    {
        (label_tabular->labels) = (label_t*) recalloc(label_tabular->labels, label_tabular->size, (size_t) INCREASE_COEFFICIENT * label_tabular->size);
        label_tabular->size = (size_t) INCREASE_COEFFICIENT * label_tabular->size;

        if (label_tabular->labels == NULL)
        {
            return LABEL_INSTRUCTION_RETURN_MEMORY_ERROR;
        }
    }

    bool search_flag = false;
    for (size_t index = 0; index < label_tabular->count; index++)
    {
        if (strcmp(label_name, (label_tabular->labels)[index].name) == 0)
        {
            if (!(label_tabular->labels)[index].is_used)
            {
                (label_tabular->labels)[index].to = instructions->instructions_count + 1;
                (label_tabular->labels)[index].is_used = true;
            }
            else
            {
                return LABEL_INSTRUCTION_RETURN_USAGE_REPEAT;
            }

            search_flag = true;

            break;
        }
    }

    if (!search_flag)
    {
        (label_tabular->labels)[label_tabular->count] = {.name = label_name, .from = 0, .to = instructions->instructions_count + 1, .is_initialized = false, .is_used = true};
        label_tabular->count++;
    }

    return LABEL_INSTRUCTION_RETURN_SUCCESS;
}

label_instruction_return_e
InitLabel(char*                    label_name,
          compiler_instructions_t* instructions,
          label_tabular_t*         label_tabular)
{
    ASSERT(label_name != NULL);
    ASSERT(instructions != NULL);
    ASSERT(label_tabular != NULL);

    if (label_tabular->count == label_tabular->size)
    {
        (label_tabular->labels) = (label_t*) recalloc(label_tabular->labels, label_tabular->size, (size_t) INCREASE_COEFFICIENT * label_tabular->size);
        label_tabular->size = (size_t) INCREASE_COEFFICIENT * label_tabular->size;

        if (label_tabular->labels == NULL)
        {
            return LABEL_INSTRUCTION_RETURN_MEMORY_ERROR;
        }
    }

    bool search_flag = false;
    for (size_t index = 0; index < label_tabular->count; index++)
    {
        if (strcmp(label_name, (label_tabular->labels)[index].name) == 0)
        {
            if (!(label_tabular->labels)[index].is_initialized)
            {
                (label_tabular->labels)[index].from = instructions->instructions_count;
                (label_tabular->labels)[index].is_initialized = true;
            }
            else
            {
                return LABEL_INSTRUCTION_RETURN_USAGE_REPEAT;
            }

            search_flag = true;

            break;
        }
    }

    if (!search_flag)
    {
        (label_tabular->labels)[label_tabular->count] = {.name = label_name, .from = instructions->instructions_count, .to = 0, .is_initialized = true, .is_used = false};
        label_tabular->count++;
    }

    return LABEL_INSTRUCTION_RETURN_SUCCESS;
}

void
LabelTabularDump(label_tabular_t* label_tabular)
{
    printf(YELLOW "||" RED "    Names" YELLOW "||" RED "     From" YELLOW "||" RED "       To" YELLOW "||" RED "   IsInit" YELLOW "||" RED "   IsUsed" YELLOW "||\n" STANDARD);

    for (size_t index = 0; index < label_tabular->count; index++)
    {
        label_t current_label = (label_tabular->labels)[index];
        printf(YELLOW "||" WHITE "%9s" YELLOW "||" WHITE "%9zu" YELLOW "||" WHITE "%9zu" YELLOW "||" WHITE "%9d" YELLOW "||" WHITE "%9d" YELLOW "||\n" STANDARD, current_label.name, current_label.from, current_label.to, current_label.is_initialized, current_label.is_used);
    }
}

void
DestroyLabelTabular(label_tabular_t* label_tabular)
{
    free(label_tabular->labels);
    free(label_tabular);
};
