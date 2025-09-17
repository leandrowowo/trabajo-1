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

/*
    VARIABLES GLOBALES
*/
#define SILENT 0
#define VERBOSE 1

/*
    FUNCIONES
*/
void Usage(char *argv[])
{
    printf("Usage: %s -O < <input archive> \n\nwhere:\n\t O in {V, S}\n", argv[0]);
    exit(1);
}

void read_data(int *m, int *n, int ***numbers)
{
    int i, j;

    scanf("%d", m); // Cantidad de dígitos del primer número
    *numbers = (int **)calloc(2, sizeof(int *)); // Memoria para dos filas

    (*numbers)[0] = (int *)calloc(*m, sizeof(int)); // Asignamos memoria para la primera fila cantidad M

    for(i = 0; i < *m; i = i + 1)
    {
        scanf("%d", &(*numbers)[0][i]); //Asignamos los valores a la primera fila
    }

    scanf("%d", n); // Cantidad de dígitos del segundo número

    (*numbers)[1] = (int *)calloc(*n, sizeof(int)); // Asignamos memoria para la segunda fila cantidad N

    for(j = 0; j < *n; j = j + 1)
    {
        scanf("%d", &(*numbers)[1][i]); //Asignamos los valores a la segunda fila
    }
}

void imprimir(int m, int n, int **numbers)
{
    printf("Primer número: ");
    for(int i = 0; i < m; i++)
    {
        printf("%d", numbers[0][i]);
    }
    printf("\n");

    printf("Segundo número: ");
    for(int i = 0; i < n; i++)
    {
        printf("%d", numbers[1][i]);
    }
    printf("\n");
}


/*
    MAIN
*/
int main(int argc, char **argv)
{
    int m, n; // Cantidad de dígitos para números de entrada
    int **numbers; // Arreglo bidimensional

    read_data(&m, &n, &numbers);
    
    if(argc != 3)
    {
        Usage(argv);
    }
    else
    {
        imprimir(m, n, numbers);

        // if(argv[1] == "-S")
        // {
        //     //tradMult_secuential();
        // }
        // else if(argv[1] == "-V")
        // {
        //     /* code */
        // }
    }

    return 0;
}
