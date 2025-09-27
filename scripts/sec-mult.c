/***************************************************************************************
 * 
 * sec-mult.c: Programa que calcula la multiplicación de dos números de manera secuencial
 *             a través del método tradicional
 *
 * Programmer: Leandro Aballay Henriquez - Delian Santis Lopez
 *
 * Santiago de Chile, 06/10/2025
 *
 **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
        scanf("%d", &(*numbers)[1][j]); //Asignamos los valores a la segunda fila
    }
}

int *Process(int **numbers, int m, int n, int *result_size, float *CPU_time, long *Wall_time)
{
    int *a, *b, *result, i, j;
    int *factor; // Arreglo que guarda las potencias de 10 correspondientes a las unidades decimales de a o b
    clock_t CPU_start, CPU_finish; // Tiempos de CPU
    time_t wall_start, wall_finish; // Tiempos de Wall

    a = numbers[0];
    b = numbers[1];

    *result_size = m + n; // Cantidad máxima de dígitos del resultado de la multiplicación
    result = (int *)calloc(*result_size, sizeof(int));

    wall_start = time(NULL);
    CPU_start = clock();
    
    // Proceso de multiplicación
    for(i = n - 1; i >= 0; i = i - 1)
    {
        for(j = m - 1; j >= 0; j = j - 1)
        {
            result[i+j+1] = result[i+j+1] + a[j] * b[i];
        }
    }

    // Proceso de acarreo
    for(i = *result_size; i > 0; i = i - 1)
    {
        if(result[i] >= 10)
        {
            result[i-1] = result[i-1] + result[i]/10;
            result[i] = result[i] % 10;
        }
    }

    // Si el primer dígito de result es 0, se elimina del arreglo
    if(result[0] == 0)
    {
        for(i = 0; i < *result_size; i = i + 1)
        {
            result[i] = result[i+1];
        }

        *result_size = *result_size - 1;
    }

    CPU_finish = clock();
    wall_finish = time(NULL);

    *Wall_time = (long)(wall_finish - wall_start); // Cálculo de tiempo de procesos (Wall time)
    *CPU_time = (float)(CPU_finish - CPU_start) / CLOCKS_PER_SEC; // Cálculo de tiempo de CPU

    return result;
}

void printData(int mode)
{
    int m, n, **numbers, i, j;
    int a, b, *resultado, resultado_size;
    float CPU_time;
    long Wall_time;

    read_data(&m, &n, &numbers);

    resultado = Process(numbers, m, n, &resultado_size, &CPU_time, &Wall_time);

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
            printf("%d", resultado[i]);
        }
        
        printf("\nTiempo de ejecución CPU (segundos): %f\n", CPU_time);
        printf("Tiempo de ejecución total (segundos): %f\n", Wall_time);
    }

    // Liberación de memoria
    free(numbers[0]);
    free(numbers[1]);
    free(numbers);
}


/*
    MAIN
*/
int main(int argc, char **argv)
{
    int mode;

    if(argc != 2)
    {
        Usage(argv);
    }
    else
    {
        if(!strcmp(argv[1], "-V"))
        {
            mode = VERBOSE;
            printData(mode);
        }
        else if(!strcmp(argv[1], "-S"))
        {
            mode = SILENT;
            printData(mode);
        }
    }

    return 0;
}
