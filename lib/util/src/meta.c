#include "util/meta.h"

static const char* _exe_path;

void meta_init(int argc, char** argv)
{
    UNUSED(argc);
    _exe_path = argv[0];
}

const char* meta_get_exe_path(void)
{
    return _exe_path;
}
