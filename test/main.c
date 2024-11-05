#include "bf_parser.h"
#include "dynarray.h"

int main(int argc, char* argv[])
{
    UNUSED(argc); UNUSED(argv);
    printf("\nUtils tests:\n");
    dynarray_test();
    printf("BF tests:\n");
    bf_parser_test();
    printf("\nTests complete! Everything passed.\n\n");
    return EXIT_STATUS_SUCCESS;
}
