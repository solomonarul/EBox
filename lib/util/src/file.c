#include "file.h"

#include <stdlib.h>

uint32_t file_get_size(FILE* file)
{
    // Seek to the end of the file, get position, reset.
    fseek(file, 0L, SEEK_END);
    uint32_t file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return file_size;
}

void* file_read_all(FILE* file)
{
    // Read everything into a buffer. This needs to be freed later.
    uint32_t file_size = file_get_size(file);
    void* input_data = calloc(file_size + 1, 1);        // This doesn't include trailing 0x00 so +1.
    fread(input_data, 1, file_size, file);
    return input_data;
}
