/******************************************************************************
 *
 * FILE: T2-PAR.C
 *
 * DESCRIPTION: Implementación paralela del algortimo ruso para la multiplicación
 *
 * AUTHOR: Delian Santis López - Leandro Aballay Henriquez
 * *
 * ENTREGA: 30/10/2025
 *
 *****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define SILENT      0
#define VERBOSE     1


/*
 * (Reemplaza tu struct Messages con esto)
 */
struct Messages {
    int id;
    int num_threads;
    int opmode; 
    int *a_work;
    int *b_work;
    int *result_work;
    int *a_size;
    int *b_start;
    int start_index;
    int end_index;
    int *all_starts;
    int max_size;
    int *is_a_odd;
    int *is_a_zero;
    int *carries;
    int *remainders;
};


int **numbers;              // arreglo bidimensional global que almacena los numeros del archivo
pthread_barrier_t barrier;  // barrera para sincronización de hilos

/*
 * 
 * 
 */
void read_data(int *m, int *n)
{
    int i, j;

    numbers = (int **)calloc(2, sizeof(int *)); // Memoria para dos filas

    scanf("%d", m); // Cantidad de dígitos del primer número
    numbers[0] = (int *)calloc(*m, sizeof(int)); // Asignamos memoria para la primera fila cantidad M

    for(i = 0; i < *m; i = i + 1)
    {
        scanf("%d", &numbers[0][i]); //Asignamos los valores a la primera fila
    }

    scanf("%d", n); // Cantidad de dígitos del segundo número
    numbers[1] = (int *)calloc(*n, sizeof(int)); // Asignamos memoria para la segunda fila cantidad N

    for(j = 0; j < *n; j = j + 1)
    {
        scanf("%d", &numbers[1][j]); //Asignamos los valores a la segunda fila
    }
}

/*
 * 
 */
void print_data(int *a_copy, int *b_copy, int m, int n, int b_start_idx){

    int i;
    
    printf("****** Archivo utilizado ******\n");
    
    /* Imprimir M (tamaño) y los dígitos de A */
    printf("%d\n", m);
    for (i = 0; i < m; i = i + 1) {
        printf("%d\n", a_copy[i]);
    }
    
    /* Imprimir N (tamaño) y los dígitos de B */
    printf("%d\n", n);
    for (i = 0; i < n; i = i + 1) {
        /* Leemos desde b_copy usando el índice de inicio (b_start_idx) */
        printf("%d\n", b_copy[b_start_idx + i]);
    }
}

/*
 * 
 */
void *Process(void *p) {
    
    struct Messages *data = (struct Messages *) p;
    
    /* --- CAMBIOS: Los chunks ahora se reciben, no se calculan --- */
    int start_index = data->start_index;
    int end_index = data->end_index;
    /* --- FIN CAMBIOS --- */
    
    int i, pos, carry_sum, sum, carry_to_propagate, remainder, actual, temp, carry_mul;

    if (data->opmode == VERBOSE) {
        printf("\n\n**************************************\n\n");
        printf("From %d - Beginning The Task (Chunk Size: %d)\n\n", data->id, (end_index - start_index));
    }

    while (!(*data->is_a_zero)) {

        // FASE 1: SUMA (SI A ES IMPAR)
        if (data->id == 0) { 
            *data->is_a_odd = (data->a_work[*(data->a_size) - 1] % 2 != 0);
        }

        pthread_barrier_wait(&barrier); 

        if (*data->is_a_odd) {
            carry_sum = 0;
            /* Usamos los nuevos start/end index */
            for (i = end_index - 1; i >= start_index; i = i - 1) {
                sum = data->result_work[i] + data->b_work[i] + carry_sum;
                data->result_work[i] = sum % 10;
                carry_sum = sum / 10;
            }
            data->carries[data->id] = carry_sum; 
        }

        pthread_barrier_wait(&barrier);

        /* --- CAMBIOS: Lógica de acarreo (Hilo 0) actualizada --- */
        if (*data->is_a_odd && data->id == 0) {
            for (i = data->num_threads - 1; i > 0; i = i - 1) {
                if (data->carries[i] > 0) { 
                    /* Ya no podemos asumir chunks iguales. Usamos el arreglo 'all_starts'. */
                    /* pos es el final del chunk anterior (inicio de este chunk - 1) */
                    pos = data->all_starts[i] - 1; 
                    
                    carry_to_propagate = data->carries[i];
                    while (pos >= 0 && carry_to_propagate > 0) {
                        sum = data->result_work[pos] + carry_to_propagate;
                        data->result_work[pos] = sum % 10;
                        carry_to_propagate = sum / 10;
                        pos = pos - 1;
                    }
                    data->carries[i] = 0; 
                }
            }
        }
        /* --- FIN CAMBIOS --- */

        pthread_barrier_wait(&barrier);

        /* (Las Fases 2, 3 y 4 del Hilo 0 no cambian) */
        if (data->id == 0) {
            remainder = 0;
            for (i = 0; i < *(data->a_size); i = i + 1) {
                 actual = remainder * 10 + data->a_work[i];
                 data->a_work[i] = actual / 2;
                 remainder = actual % 2;
            }
            while (*(data->a_size) > 1 && data->a_work[0] == 0) {
                for (i = 0; i < *(data->a_size) - 1; i = i + 1) {
                    data->a_work[i] = data->a_work[i+1];
                }
                *(data->a_size) = *(data->a_size) - 1;
            }
        }
        pthread_barrier_wait(&barrier);
        if (data->id == 0) {
            carry_mul = 0;
            for (i = data->max_size - 1; i >= *(data->b_start) - 1 && i >= 0; i = i - 1) { 
                temp = data->b_work[i] * 2 + carry_mul;
                data->b_work[i] = temp % 10;
                carry_mul = temp / 10;
            }
            if(data->b_work[*(data->b_start)-1] > 0) {
                 *(data->b_start) = *(data->b_start) - 1;
            }
        }
        pthread_barrier_wait(&barrier);
        if (data->id == 0) {
            if (*(data->a_size) == 1 && data->a_work[0] == 0) {
                *data->is_a_zero = 1;
            }
        }
        pthread_barrier_wait(&barrier);
    }
    
    if (data->opmode == VERBOSE) {
        printf("From %d Ending The Task\n\n", data->id);
    }

    pthread_exit(NULL);
}

/*
 * (Reemplaza tu función Russian con esto)
 */
void Russian(int k, int mode) {

    int m, n, i;
    clock_t CPU_start, CPU_finish;
    time_t wall_start, wall_finish;
    long wall_time;
    float CPU_time;
    int max_size, a_size, b_size, b_start, start_print;
    int *a_copy, *b_copy, *result;
    int *carries, *remainders;
    int is_a_odd, is_a_zero;
    
    pthread_t *thread;
    struct Messages **mess;
    void *exit_status;
    pthread_attr_t attribute;
    
    int s;  // tamaño base del chunk 
    int rem;// resto de chunks 
    int l;  //índice de inicio actual (running index)
    
    // Arreglo para guardar los inicios (para el Hilo 0)
    int *offsets; 

    read_data(&m, &n);

    max_size = m + n + 1;
    a_size = m;
    b_size = n;
    b_start = max_size - b_size;

    result = (int *)calloc(max_size, sizeof(int));
    a_copy = (int *)calloc(a_size, sizeof(int));
    b_copy = (int *)calloc(max_size, sizeof(int));
    offsets = (int *)calloc(k, sizeof(int));

    for (i = 0; i < a_size; i = i + 1) {
        a_copy[i] = numbers[0][i];
    }
    for (i = 0; i < b_size; i = i + 1) {
        b_copy[b_start + i] = numbers[1][i];
    }
    
    free(numbers[0]);
    free(numbers[1]);
    free(numbers);
    
    if (mode == VERBOSE) {
        print_data(a_copy, b_copy, m, n, b_start);
    }

    wall_start = time(NULL);
    CPU_start = clock();

    is_a_odd = 0;
    is_a_zero = 0;
    if (a_size == 1 && a_copy[0] == 0) {
        is_a_zero = 1;
    }

    pthread_barrier_init(&barrier, NULL, k);

    pthread_attr_init(&attribute);
    thread = (pthread_t *)calloc(k, sizeof(pthread_t));
    pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE);

    mess = (struct Messages **)calloc(k, sizeof(struct Messages *));
    for (i = 0; i < k; i = i + 1)
        mess[i] = (struct Messages *)calloc(1, sizeof(struct Messages));
    carries = (int *)calloc(k, sizeof(int));
    remainders = (int *)calloc(k, sizeof(int));
    
    //Repartición de chunks
    s = max_size / k;
    rem = max_size % k;
    l = 0;

    //Llenado de datos
    for (i = 0; i < k; i = i + 1) {
        
        int current_chunk_size;
        if (rem > 0) {
            current_chunk_size = s + 1;
            rem = rem - 1;
        } else {
            current_chunk_size = s;
        }
        offsets[i] = l;
        mess[i]->start_index = l;
        mess[i]->end_index = l + current_chunk_size;
        l = l + current_chunk_size;
        
        mess[i]->id = i;
        mess[i]->num_threads = k; 
        mess[i]->a_work = a_copy;
        mess[i]->b_work = b_copy;
        mess[i]->result_work = result;
        mess[i]->a_size = &a_size;
        mess[i]->b_start = &b_start;
        mess[i]->all_starts = offsets;
        mess[i]->max_size = max_size; 
        mess[i]->is_a_odd = &is_a_odd;
        mess[i]->is_a_zero = &is_a_zero;
        mess[i]->carries = carries;
        mess[i]->remainders = remainders;
        mess[i]->opmode = mode;
        
        if (mode == VERBOSE) {
            printf("Main: creating thread %d\n", i);
        }
        pthread_create(&thread[i], &attribute, Process, (void *)mess[i]);
    }

    pthread_attr_destroy(&attribute);

    for (i = 0; i < k; i = i + 1) {
        pthread_join(thread[i], &exit_status);
    }

    CPU_finish = clock();
    wall_finish = time(NULL);
    wall_time = (long)(wall_finish - wall_start);
    CPU_time = (float)(CPU_finish - CPU_start) / CLOCKS_PER_SEC;

    pthread_barrier_destroy(&barrier);

    start_print = 0;
    while (start_print < max_size - 1 && result[start_print] == 0) {
        start_print = start_print + 1;
    }

    if (mode == VERBOSE){
        printf("\n\tResult: ");
        if (start_print == max_size && result[max_size-1] == 0) {
            printf("0");
        } else {
            for (i = start_print; i < max_size; i = i + 1) {
                printf("%d", result[i]);
            }
        }
    }
    
    printf("\n");
    printf("\nMétodo: Ruso Paralelo (%d hilos)\n", k);
    printf("Tiempo de ejecución CPU (segundos): %f\n", CPU_time);
    printf("Tiempo de ejecución WallTime (segundos): %ld\n", wall_time);

    /* Liberar memoria de trabajo */
    free(carries);
    free(remainders);
    free(offsets);
    
    for (i = 0; i < k; i = i + 1)
        free(mess[i]);

    free(mess);
    free(thread);
    free(result);
    free(a_copy);
    free(b_copy);
}