/******************************************************************************
 * 
 * FILE: t1.c 
 *
 * DESCRIPTION: Programa que permite calcular de manera secuencial la multiplicación 
 *              entre dos números 'a' y 'b' de 'm' y 'n' dígitos, respectivamente, 
 *              empleando dos métodos de multiplicación:
 *              - Método Tradicional
 *              - Método Ruso
 * 
 * AUTHOR: Delian Santis López - Leandro Aballay Henriquez
 * 
 * ENTREGA: 06/10/2025
 * 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "sec-trad-mult.h"

#define SILENT      0
#define VERBOSE     1

#define TRADITIONAL 0
#define RUSSIAN     1


 /*
 *
 *
 */

void Usage(char *message) {
  
   printf("\nUsage: %s -M -O < datafile",message);
   printf("\n\nM in {T, R} \t(T: Traditional, R: Russian )");
   printf("\n\nO in {S, V} \t (S: Silent, V: Verbose) \n\n");
}

/*
*
*
*/

// readData: función que lee los datos desde el archivo de entrada
void readData(int *m, int *n, int ***numbers)
{
    int i, j;

    *numbers = (int **)calloc(2, sizeof(int *)); // Memoria para dos filas

    scanf("%d", m); // Cantidad de dígitos del primer número
    (*numbers)[0] = (int *)calloc(*m, sizeof(int)); // Asignamos memoria para la primera fila cantidad M

    for(i = 0; i < *m; i = i + 1)
    {
        scanf("%d", &(*numbers)[0][i]); //Asignamos los valores a la primera fila
    }

    scanf("%d", n); // Cantidad de dígitos del segundo número
    (*numbers)[1] = (int *)calloc(*n, sizeof(int)); // Asignamos memoria para la segunda fila cantidad N

    for(j = 0; j < *n; j = j + 1)
    {
        scanf("%d", &(*numbers)[1][j]); // Asignamos los valores a la segunda fila
    }
}


/*
*
*
*/

void printData(int mode, int m, int n, int **numbers, int *resultado, int resultado_size, float CPU_time, long Wall_time)
{
    int i, j; 

    if(mode == SILENT)
    {
        printf("Número de dígitos primer valor: %d\n", m);
        printf("Número de dígitos segundo valor: %d\n", n);
        printf("Tiempo de ejecución CPU (segundos): %f\n", CPU_time);
        printf("Tiempo de ejecución total (segundos): %f\n", Wall_time);
    }
    else if(mode == VERBOSE)
    {
        printf("Contenido del archivo\n");
        printf("---------------------\n");

        printf("%d\n", m); // Imprime cantidad de dígitos del primer valor de entrada
        for(i = 0; i < m; i = i + 1)
        {
            printf("%d\n", numbers[0][i]); // Imprime los dígitos del primer valor
        }

        printf("%d\n", n); // Imprime cantidad de dígitos del segundo valor de entrada
        for(j = 0; j < n; j = j + 1)
        {
            printf("%d\n", numbers[1][j]); // Imprime los dígitos del segundo valor
        }

        printf("Resultado: ");
        for (i = 0; i < resultado_size; i = i + 1)
        {
            printf("%d", resultado[i]); // Imprime los dígitos del resultado de la multiplicación
        }
        
        printf("\nTiempo de ejecución CPU (segundos): %f\n", CPU_time);
        printf("Tiempo de ejecución total (segundos): %f\n", Wall_time);
    }
}

 /*
 * 
 *
 */
int main(int argc, char **argv)
{
    int m, n, **numbers;
    int *result, result_size;
    int mode, method;
    int i, j;
    float CPU_time;
    long Wall_time;

    if(argc == 3)
    {
        readData(&m, &n, &numbers);

        if(strcmp(argv[1], "-T") == 0)
        {
            method = TRADITIONAL;

            if(strcmp(argv[2], "-S") == 0)
            {
                mode = SILENT;
            }
            else if(strcmp(argv[2], "-V") == 0)
            {
                mode = VERBOSE;
            }

            result_size = m + n;
            result = Process_tradMult(numbers, m, n, &CPU_time, &Wall_time);

            // Proceso de acarreo
            for(i = result_size - 1; i > 0; i = i - 1)
            {
                if(result[i] >= 10)
                {
                    result[i-1] = result[i-1] + result[i]/10;
                    result[i] = result[i] % 10;
                }
            }

            // Eliminación de ceros al inicio del arreglo
            if(result[0] == 0)
            {
                for(i = 0; i < result_size; i = i + 1)
                {
                    result[i] = result[i+1];
                }

                result_size = result_size - 1;
            }

        }
        else if(strcmp(argv[1], "-R") == 0)
        {
            method = RUSSIAN;

            if(strcmp(argv[2], "-S") == 0)
            {
                mode = SILENT;
            }
            else if(strcmp(argv[2], "-V") == 0)
            {
                mode = VERBOSE;
            }
        }

        printData(mode, m, n, numbers, result, result_size, CPU_time, Wall_time);

        // Liberar memoria
        free(result);
        free(numbers[0]);
        free(numbers[1]);
        free(numbers);

    }
    else
    {
        Usage(argv[0]);
        exit(1);
    }

    return 0;
}