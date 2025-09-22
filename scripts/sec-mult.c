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


void transformation(int **numbers, int m, int n, unsigned int *num1, unsigned int *num2)
{
    int i, j;

    *num1 = 0;
    *num2 = 0;

    for(i = 0; i < m; i = i + 1)
    {
        *num1 = (*num1 * 10) + numbers[0][i];
    }

    for(j = 0; j < n; j = j + 1)
    {
        *num2 = (*num2 * 10) + numbers[1][j];
    }
}

unsigned long long traditional_mult(unsigned int a, unsigned int b, float *CPUtime)
{
    unsigned int digit;
    unsigned long long partial_result;
    unsigned long long result = 0;
    unsigned long long factor = 1; // Equivale a las potencias de 10 al momento de multiplicar y correrse un espacio
    clock_t start, finish; // Tiempos de CPU

    start = clock();
    while(b > 0)
    {
        digit = b % 10;     // Toma el último dígito del segundo factor
        partial_result = (unsigned long long)a * digit;     // 'a' se convierte a unsigned long long para evitar overflow en la multiplicación
        result = result + partial_result * factor;
        b = b / 10;     // Divide por 10 para tomar el siguiente digito a multiplicar en la próxima iteración
        factor = factor * 10;
    }
    finish = clock();

    *CPUtime = (float)(finish - start)/CLOCKS_PER_SEC; 

    return result;
}

void printData(int mode)
{
    int m, n, **numbers, i, j;
    unsigned int a, b;
    unsigned long long resultado;
    clock_t start, finish;
    float CPU_time, wall_time;

    start = clock();
    read_data(&m, &n, &numbers);

    transformation(numbers, m, n, &a, &b);

    resultado = traditional_mult(a, b, &CPU_time);
    finish = clock();

    wall_time = (float)(finish - start)/CLOCKS_PER_SEC; // Cálculo de tiempo de procesos (Wall time)

    if(mode == SILENT)
    {
        printf("Número de dígitos primer valor: %d\n", m);
        printf("Número de dígitos segundo valor: %d\n", n);
        printf("Tiempo de ejecución CPU (segundos): %f\n", CPU_time);
        printf("Tiempo de ejecución total (segundos): %f\n", wall_time);
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

        printf("Resultado: %llu\n", resultado);
        printf("Tiempo de ejecución CPU (segundos): %f\n", CPU_time);
        printf("Tiempo de ejecución total (segundos): %f\n", wall_time);
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
