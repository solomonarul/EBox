#include "bf.h"

int main(int argc, char* argv[])
{
    printf("BF benchmarks:\n");
    bf_run_bench();
    printf("\nBenchmark complete.\n\n");
    return EXIT_STATUS_SUCCESS;
}