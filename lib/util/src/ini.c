#include "util/ini.h"

#include <stdlib.h>

char const* ini_data_to_string(ini_data_t* data)
{
    UNUSED(data);
    char const* result = calloc(1, sizeof(char));

    return result;
}

uint32_t ini_data_to_integer(ini_data_t* data)
{
    UNUSED(data);
    return 0;  
}

