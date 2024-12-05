#pragma once

#include <stdio.h>
#include "dynarray.h"

typedef struct {
    dynarray_t sections;
} ini_file_t;

typedef struct {
    char *name;
    dynarray_t data;
} ini_section_t;

typedef struct {
    char *name;
    char *value;
} ini_data_t;

ini_file_t ini_file_read(FILE* file);
void ini_file_free(ini_file_t ini);

void ini_file_write(FILE* file, ini_file_t ini);
void ini_file_write_section(ini_section_t section, FILE* file);
void ini_file_write_data(ini_data_t data, FILE* file);

ini_section_t* ini_file_get_section(ini_file_t ini, const char* sectionName);
ini_data_t* ini_section_get_data(ini_section_t section, const char* dataName);
