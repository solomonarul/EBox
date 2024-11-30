#pragma once

#include "dynarray.h"
#include <stdint.h>

// INI file structure:
// lines starting with ; are comments, and so, ignored.
// [sections]
// key=value

#define MAX_LINE_SIZE 255

typedef struct {
    dynarray_t sections;
} ini_file_t;

typedef struct {
    dynarray_t data; 
} ini_section_t;

typedef struct {
    dynarray_t key;
    void* value;
    enum {
        STRING = 0,
        INTEGER
    } value_type;
} ini_data_t;

const char* ini_data_to_string(ini_data_t* data);
uint32_t ini_data_to_integer(ini_data_t* data);
