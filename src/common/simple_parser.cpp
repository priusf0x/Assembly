#include "simple_parser.h"

#include <assert.h>

#include "stdlib.h"

enum read_flags_return_e
ReadFlags(int                argc,
          const char* const* argv,
          const char**       input_name,
          const char**       output_name)
{
    assert(argv != NULL);
    assert(input_name != NULL);
    assert(output_name != NULL);

    #pragma GCC diagnostic ignored "-Wimplicit-fallthrough" 

    switch(argc)
    {
        case 3:
            (*output_name) = argv[2];
        case 2:
            (*input_name) = argv[1];
        case 1:
            return READ_FLAGS_RETURN_SUCCESS;

        default:return READ_FLAGS_RETURN_ERROR;
    }
    
    #pragma GCC diagnostic warning "-Wimplicit-fallthrough" 
}
