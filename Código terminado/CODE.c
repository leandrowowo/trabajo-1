/******************************************************************************
 *
 * FILE: CODE.c
 *
 * DESCRIPTION: Implementación del programa principal que selecciona el método de multiplicación (RUSA O TRADICIONAL)
 *
 * AUTHOR: Delian Santis López - Leandro Aballay Henriquez
 * 
 * ENTREGA: 30/10/2025
 *
 *****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "T2-PAR.h" //En windows pthread esta mal, pero con subsistema ubuntu funciona perfectamente
#include "par-trad-mult.h"

#define RUSSIAN 2
#define TRADITIONAL 3

void Usage(char *message) {
    printf("\nUsage: %s k -M -O < datafile",message);
    printf("\n\nk: Theads Number");
    printf("\n\nM in {R, T} \t(R: Ruso Paralelo, T: Tradicional Paralelo)");
    printf("\n\nO in {S,V} \t (S: Silence, V: Verbose) \n\n");
}

int main(int argc, char **argv) {

    int k, mode, method;

    k = atoi(argv[1]);

    if (strcmp(argv[2], "-R") == 0)
        method = RUSSIAN;
    else if (strcmp(argv[2], "-T") == 0)
        method = TRADITIONAL;
    else {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[3], "-S") == 0)
        mode = SILENT;
    else if (strcmp(argv[3], "-V") == 0)
        mode = VERBOSE;
    else {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    //Llamada a los métodos según selección (RUSSIAN O TRADITIONAL)
    if(method == RUSSIAN) {
        Russian(k, mode);
    }
    else if(method == TRADITIONAL) {
        Traditional(k, mode);
    }

    return 0;
}