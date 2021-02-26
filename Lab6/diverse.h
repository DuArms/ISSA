#pragma once

#include "comunication.h"

void print_as_hex(const uint8_t *vector)
{
    for (int i = 0; i < MSG_LEN; i++)
    {
        printf(" %02x ", vector[i]);
    }
    printf("\n");
    
}

void copy_a2b(uint8_t *a, uint8_t *b)
{
    for (int i = 0; i < MSG_LEN; i++)
    {
        b[i] = a[i];
    }
}