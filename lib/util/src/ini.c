#include "ini.h"
#include "dynarray.h"

#include <string.h>
#include <stdlib.h>

#define INI_MAX_LINE_LENGTH 256

ini_file_t ini_file_read(FILE* file)
{
    ini_file_t result;
    dynarray_init(&result.sections, sizeof(ini_section_t), 0, NULL);

    // Read line by line.
    char line[INI_MAX_LINE_LENGTH];
    while (fgets(line, INI_MAX_LINE_LENGTH, file) != NULL)
    {
        char* lineStart = line;
        while(*lineStart == ' ' || *lineStart == '\t')
            lineStart++;
        char* lineEnd = line + (strlen(lineStart) - 1);
        while(lineEnd >= lineStart && (*lineEnd == ' ' || *lineEnd == '\t' || *lineEnd == '\n'))
            lineEnd--;
        
        // Nothing new.
        if (lineEnd < lineStart)
            continue;

        // Check if it's a section.
        if(*lineStart == '[' && *lineEnd == ']')
        {
            // Create a new section.
            ini_section_t section;
            section.name = calloc(lineEnd - lineStart, sizeof(char));
            strncpy(section.name, lineStart + 1, lineEnd - lineStart - 1);
            dynarray_init(&section.data, sizeof(ini_data_t), 0, NULL);
            dynarray_push_back(&result.sections, &section);
        }
        else
        {
            // Check if it's a data line.
            char* equal = strchr(lineStart, '=');
            if (equal != NULL)
            {
                // Create a new data.
                ini_data_t data;
                data.name = calloc(equal - lineStart + 1, sizeof(char));
                strncpy(data.name, lineStart, equal - lineStart);
                data.value = calloc(lineEnd - equal + 1, sizeof(char));
                strncpy(data.value, equal + 1, lineEnd - equal);
                
                // Get last section.
                ini_section_t* section = dynarray_get(result.sections, result.sections.size - 1);
                dynarray_push_back(&section->data, &data);
            }
        }
    }

    return result;
}

void ini_file_free(ini_file_t ini)
{
    for (size_t sectionIndex = 0; sectionIndex < ini.sections.size; sectionIndex++)
    {
        ini_section_t* section = dynarray_get(ini.sections, sectionIndex);
        for (size_t dataIndex = 0; dataIndex < section->data.size; dataIndex++)
        {
            ini_data_t* data = dynarray_get(section->data, dataIndex);
            free(data->name);
            free(data->value);
        }
        dynarray_free(section->data);
        free(section->name);
    }
    dynarray_free(ini.sections);
}

void ini_file_write(FILE* file, ini_file_t ini)
{
    for(size_t sectionIndex = 0; sectionIndex < ini.sections.size; sectionIndex++)
    {
        ini_section_t* section = dynarray_get(ini.sections, sectionIndex);
        ini_file_write_section(*section, file);
    }
}

void ini_file_write_section(ini_section_t section, FILE* file)
{
    fprintf(file, "[%s]\n", section.name);
    for(size_t dataIndex = 0; dataIndex < section.data.size; dataIndex++)
    {
        ini_data_t* data = dynarray_get(section.data, dataIndex);
        ini_file_write_data(*data, file);
    }
    fprintf(file, "\n");
}

void ini_file_write_data(ini_data_t data, FILE* file)
{
    fprintf(file, "%s=%s\n", data.name, data.value);
}


ini_section_t* ini_file_get_section(ini_file_t ini, const char* sectionName)
{
    for(size_t sectionIndex = 0; sectionIndex < ini.sections.size; sectionIndex++)
    {
        ini_section_t* section = dynarray_get(ini.sections, sectionIndex);
        if(strcmp(section->name, sectionName) == 0)
            return section;
    }
    return NULL;
}

ini_data_t* ini_section_get_data(ini_section_t section, const char* dataName)
{
    for(size_t dataIndex = 0; dataIndex < section.data.size; dataIndex++)
    {
        ini_data_t* data = dynarray_get(section.data, dataIndex);
        if(strcmp(data->name, dataName) == 0)
            return data;
    }
    return NULL;
}
