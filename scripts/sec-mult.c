/***************************************************************************************
 * 
 * mult_trad_sec.c: Programa que calcula la multiplicación de dos números de manera secuencial
 *                  a través del método tradicional
 *
 * Programmer: Leandro Aballay Henriquez - Delian Santis Lopez
 *
 * Santiago de Chile, 06/10/2025
 *
 **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void Usage(char *argv[])
{
    printf("Usage: %s -M -O < <input archive> \n\nwhere:\t M in {T, R} \n\t O in {V, S}\n", argv[0]);
    exit(1);
}

int main(int argc, char **argv)
{
    Usage(argv);

    return 0;
}
