#include "ui/cli.h"

#include <stdio.h>

void bf_print_to_cli(uint8_t character)
{
    printf("%c", character);
}

uint8_t bf_read_from_cli(void)
{
    uint8_t character;
    fscanf(stdin, "%c", &character);
    return character;
}
