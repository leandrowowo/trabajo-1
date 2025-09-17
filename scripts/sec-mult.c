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

// Definición variables 
#define SILENT 0
#define VERBOSE 1
#define SECUENTIAL 2
#define PARALLEL 3

void Usage(char *argv[])
{
    printf("Usage: %s -M -O < <input archive> \n\nwhere:\t M in {S, P} \n\t O in {V, S}\n", argv[0]);
    exit(1);
}

void readData(int *m, int *n, int **numbers)
{

}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        Usage(argv);
    }
    else
    {
        if(argv[1] == "-S" && argv[2] == "-S")
        {
            //tradMult_secuential();
        }
        else if(argv[1] == "-S" && argv[2] == "-V")
        {
            /* code */
        }
        else if(argv[1] == "-P" && argv[2] == "-S")
        {
            /* code */
        }
        else if(argv[1] == "-P" && argv[2] == "-V")
        {
            /* code */
        }
        
    }

    return 0;
}
