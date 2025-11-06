#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Process: función que realiza la multiplicación
int *Process_tradMult(int **numbers, int m, int n, float *CPU_time, long *Wall_time)
{
    int *a, *b, *result, i, j;
    clock_t CPU_start, CPU_finish; // Tiempos de CPU
    time_t wall_start, wall_finish; // Tiempos de Wall

    a = numbers[0];
    b = numbers[1];
    
    result = (int *)calloc(m + n, sizeof(int));

    wall_start = time(NULL);
    CPU_start = clock();
    // Proceso de multiplicación
    for(i = n - 1; i >= 0; i = i - 1)
    {
        for(j = m - 1; j >= 0; j = j - 1)
        {
            result[i+j+1] = result[i+j+1] + b[i] * a[j];
        }
    }
    CPU_finish = clock();
    wall_finish = time(NULL);

    *Wall_time = (long)(wall_finish - wall_start); // Cálculo de tiempo de procesos (Wall time)
    *CPU_time = (float)(CPU_finish - CPU_start) / CLOCKS_PER_SEC; // Cálculo de tiempo de CPU

    return result;
}