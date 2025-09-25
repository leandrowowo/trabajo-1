/***************************************************************************************
 * 
 * par-mult.c: Programa que calcula la multiplicación de dos números de manera paralelizada
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
#include <pthread.h>

/*
    DEFINE
*/
#define SILENT 0
#define VERBOSE 1

/*
    STRUCT
*/
struct Message
{
    int myid, ndigits, size, opmode;
    unsigned long long partial_result;
};

unsigned long long result;
unsigned int a, factor;

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

void transformation(int **numbers, int m, unsigned int *num1)
{
    int i, j;

    *num1 = 0;

    for(i = 0; i < m; i = i + 1)
    {
        *num1 = (*num1 * 10) + numbers[0][i];
    }
}

void Process(void *p)
{
    int i, j, k;
    int m; // Cantidad de dígitos del primer valor
    int **numbers; // Arreglo bidimensional
    struct Message *me;

    me = (struct Message *) p;

    read_data(&m ,&(me->ndigits), &numbers); // Lectura de archivo
    transformation(numbers, m, &a); // Transformación del primer valor de entrada de arreglo con dígitos a número entero

    if(me->opmode == SILENT)
    {
        printf("***************************\n");
        printf("Cantidad de dígitos primer valor: %d", m);
        printf("Cantidad de dígitos segundo valor: %d", me->ndigits);

    }
}

int main()
{


    return 0;
}