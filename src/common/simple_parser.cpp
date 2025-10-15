#include "simple_parser.h"

#include "stdlib.h"
#include "Assert.h"

enum read_flags_return_e
ReadFlags(int                argc,
          const char* const* argv,
          const char**       input_name,
          const char**       output_name)
{
    ASSERT(argv != NULL);
    ASSERT(input_name != NULL);
    ASSERT(output_name != NULL);

    if (argc == 1)
    {
        return READ_FLAGS_RETURN_SUCCESS;
    }

    if (argc ==  2)
    {
        (*input_name) = argv[1];
        return READ_FLAGS_RETURN_SUCCESS;
    }

    (*input_name) = argv[1];
    (*output_name) = argv[2];

    return READ_FLAGS_RETURN_SUCCESS;
}
