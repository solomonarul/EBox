#pragma once

#include "defines.h"
#include <stdio.h>

uint32_t file_get_size(FILE* file);
void* file_read_all(FILE* file);
