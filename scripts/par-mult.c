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
    VARIABLES GLOBALES
*/
#define SILENT 0
#define VERBOSE 1

int *a, *b; // Números a multiplicar 'a' y 'b' 
int m, n;   // Tamaños 'm' y 'n' de 'a' y 'b', respectivamente


struct Message
{
    int thread_id; // Identificador del hilo
    int *partial_result; // Arreglo con resultado parcial en cada hilo
    int chunk_size; // Tamaño del chunnumthreads
    int index; // Índice de cada hilo
};


/*
    FUNCIONES
*/
void Usage(char *argv[])
{
    printf("Usage: %s -O -numthreads < <input archive> \n\nwhere:\n\t O in {V, S}\n\t numthreads: threads number\n", argv[0]);
    exit(1);
}


void readData()
{
    int i, j;

    // Lee el primer número para a
    scanf("%d", &m);
    a = (int *)calloc(m, sizeof(int));
    for(i = 0; i < m; i = i + 1)
    {
        scanf("%d", &a[i]);
    }

    // Lee el segundo número para b
    scanf("%d", &n);
    b = (int *)calloc(n, sizeof(int));
    for(j = 0; j < n; j = j + 1)
    {
        scanf("%d", &b[j]);
    }
}

void *Process(void *p)
{
    struct Message *me;
    int i, j;
    int cindex_start, cindex_finish; // Variables para el control de índices de los hilos
    clock_t CPU_start, CPU_finish; // Tiempo inicial y final de CPU
    time_t wall_start, wall_finish; // TIempo inicial y final de Wall
    float CPU_time; 
    long wall_time;

    me = (struct Message *)p;
    cindex_start = me->index;
    cindex_finish = me->index - me->chunk_size + 1;

    wall_start = time(NULL);
    CPU_start = clock();

    // Cálculo de la multiplicación
    for(i = n - 1; i >= 0; i = i - 1)
    {
        for(j = cindex_start; j >= cindex_finish; j = j - 1)
        {
            me->partial_result[i+j+1] = me->partial_result[i+j+1] + b[i] * a[j];
        }
    }

    CPU_finish = clock();
    wall_finish = time(NULL);
    
    CPU_time = (float)(CPU_finish - CPU_start) / CLOCKS_PER_SEC;
    wall_time = (long)(wall_finish - wall_start);

    printf("From %d - Times: CPU time %f Wall time %lf\n", me->thread_id, CPU_time, wall_time);

    pthread_exit(NULL);
}


void printData(int mode, int *result, int result_size)
{
    int i, j;

    if(mode == SILENT)
    {
        printf("Número de dígitos primer valor: %d\n", m);
        printf("Número de dígitos segundo valor: %d\n", n);
    }
    else if(mode == VERBOSE)
    {
        printf("\nContenido del archivo\n");
        printf("---------------------\n");

        printf("%d\n", m);
        for(i = 0; i < m; i = i + 1)
        {
            printf("%d\n", a[i]);
        }

        printf("%d\n", n);
        for(j = 0; j < n; j = j + 1)
        {
            printf("%d\n", b[j]);
        }

        printf("\nResultado: ");
        for (i = 0; i < result_size; i = i + 1)
        {
            printf("%d", result[i]);
        }
        printf("\n");
    }
}


/*
    MAIN
*/
int main(int argc, char **argv)
{
    pthread_t *thread; // Arreglo de hilos
    pthread_attr_t attribute;
    struct Message **mess;
    int mode, numthreads, i, j, k, result_size, chunk_size, rem, index;
    int **resultado_parcial, *resultado;
    void *exit_status;

    if(argc != 3 || (strcmp(argv[1], "-V") && strcmp(argv[1], "-S")))
    {
        Usage(argv);
    }
    else
    {
        if(!strcmp(argv[1], "-V"))
        {
            mode = VERBOSE;
        }
        else if(!strcmp(argv[1], "-S"))
        {
            mode = SILENT;
        }
        
        numthreads = atoi(argv[2]);

        readData();

        result_size = m + n;
        thread = calloc(numthreads, sizeof(pthread_t));
        resultado_parcial = calloc(numthreads, sizeof(int *));
        resultado = calloc(result_size, sizeof(int));
        mess = calloc(numthreads, sizeof(struct Message *));
        for(i = 0; i < numthreads; i = i + 1)
        {
            mess[i] = calloc(1, sizeof(struct Message));
            resultado_parcial[i] = calloc(result_size, sizeof(int));
        }

        pthread_attr_init(&attribute);
        pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE);
        
        chunk_size = m / numthreads;
        rem = m % numthreads;
        index = m-1;

        // Cálculo de multiplicaciones parciales
        for(i = 0; i < numthreads; i = i + 1)
        {
            mess[i]->thread_id = i;

            if(rem != 0)
            {
                mess[i]->chunk_size = chunk_size + 1;
                rem = rem - 1;
            }
            else
            {
                mess[i]->chunk_size = chunk_size;
            }

            mess[i]->partial_result = resultado_parcial[i];
            mess[i]->index = index;
            index = index - mess[i]->chunk_size;

            pthread_create(&thread[i], &attribute, Process, (void *)mess[i]);
        }

        pthread_attr_destroy(&attribute);

        for (i = 0; i < numthreads; i = i + 1)
        {
            pthread_join(thread[i],&exit_status);
        }

        // Suma de resultados parciales
        for(i = 0; i < numthreads; i = i + 1)
        {
            for(j = result_size - 1; j >= 0; j = j - 1)
            {
                resultado[j] = resultado[j] + mess[i]->partial_result[j];
            }
        }

        // Proceso de acarreo
        for(i = result_size - 1; i > 0; i = i - 1)
        {
            if(resultado[i] >= 10)
            {
                resultado[i-1] = resultado[i-1] + resultado[i]/10;
                resultado[i] = resultado[i] % 10;
            }
        }

        // Eliminación de 0 sobrantes
        if(resultado[0] == 0)
        {
            for(i = 0; i < result_size - 1; i = i + 1)
            {
                resultado[i] = resultado[i+1];
            }

            result_size = result_size - 1;
        }

        printData(mode, resultado, result_size);
    }

    return 0;
}