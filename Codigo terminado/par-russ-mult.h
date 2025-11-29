/******************************************************************************
 *
 * FILE: T2-PAR.C
 *
 * DESCRIPTION: Implementación paralela del algortimo ruso para la multiplicación
 * 
 * AUTHOR: Delian Santis López - Leandro Aballay Henriquez
 * 
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

struct Messages {
    int id;                 // Identificador del hilo
    int num_threads;        // Número total de hilos 
    int opmode;             // Modo de operación 
    int *a_work;            // Puntero al arreglo del número A (el que se divide)
    int *b_work;            // Puntero al arreglo del número B (el que se duplica)
    int *result_work;       // Puntero al arreglo del resultado
    int *a_size;            // Puntero al tamaño actual de A 
    int *b_start;           // Puntero al índice donde comienza B 
    int start_index;        // Índice de inicio del chunk para este hilo
    int end_index;          // Índice de fin del chunk para este hilo
    int *all_starts;        // Arreglo con los índices de inicio de todos los hilos 
    int max_size;           // Tamaño máximo de los arreglos (m + n + 1)
    int *is_a_odd;          // Puntero a la bandera que indica si A es impar
    int *is_a_zero;         // Puntero a la bandera que indica si A es cero 
    int *carries;           // Arreglo para almacenar los acarreos de la suma de cada hilo
    int *remainders;        // Arreglo para almacenar los restos
};

// Variables globales
int **numbers;              // Arreglo bidimensional global para leer los números del archivo
pthread_barrier_t barrier;  // Barrera para sincronización de hilos


void read_data(int *m, int *n)
{
    int i, j;

    // Pide memoria para 2 filas (un número por fila)
    numbers = (int **)calloc(2, sizeof(int *)); 

    // Lee el tamaño del primer número (M)
    scanf("%d", m); 
    // Pide memoria para M dígitos
    numbers[0] = (int *)calloc(*m, sizeof(int)); 

    // Lee dígito por dígito el primer número
    for(i = 0; i < *m; i = i + 1)
    {
        scanf("%d", &numbers[0][i]); //Asignamos los valores a la primera fila
    }

    // Lee el tamaño del segundo número (N)
    scanf("%d", n); 
    // Pide memoria para N dígitos
    numbers[1] = (int *)calloc(*n, sizeof(int)); 

    // Lee dígito por dígito el segundo número
    for(j = 0; j < *n; j = j + 1)
    {
        scanf("%d", &numbers[1][j]); //Asignamos los valores a la segunda fila
    }
}

void print_data(int *a_copy, int *b_copy, int m, int n, int b_start_idx){

    int i;
    
    printf("****** Archivo utilizado ******\n");
    
    //Imprimir M (tamaño) y los dígitos de A 
    printf("%d\n", m);
    for (i = 0; i < m; i = i + 1) {
        printf("%d\n", a_copy[i]);
    }
    
    // Imprimir N (tamaño) y los dígitos de B 
    printf("%d\n", n);
    for (i = 0; i < n; i = i + 1) {
        //Leemos desde b_copy usando el índice de inicio (b_start_idx)
        printf("%d\n", b_copy[b_start_idx + i]);
    }
}

void *Process(void *p) {
    
    struct Messages *data = (struct Messages *) p;
    int start_index = data->start_index;
    int end_index = data->end_index;
    int i, pos, carry_sum, sum, carry_to_propagate, remainder, actual, temp, carry_mul;

    if (data->opmode == VERBOSE) {
        printf("\n\n**************************************\n\n");
        printf("From %d - Beginning The Task (Chunk Size: %d)\n\n", data->id, (end_index - start_index));
    }

    // Bucle principal del algoritmo ruso: mientras A no sea 0
    while (!(*data->is_a_zero)) {
        
        // El hilo 0 comprueba si A es impar (mirando el último dígito)
        if (data->id == 0) { 
            *data->is_a_odd = (data->a_work[*(data->a_size) - 1] % 2 != 0);
        }

        // Sincronización: todos los hilos esperan aquí a que el hilo 0 termine de chequear 
        pthread_barrier_wait(&barrier); 

        // Si A es impar, todos los hilos suman su chunk de B al resultado
        if (*data->is_a_odd) {
            carry_sum = 0;
            // Cada hilo procesa su chunk asignado (de derecha a izquierda)
            for (i = end_index - 1; i >= start_index; i = i - 1) {
                sum = data->result_work[i] + data->b_work[i] + carry_sum;
                data->result_work[i] = sum % 10; // Guarda el dígito
                carry_sum = sum / 10;          // Guarda el acarreo para la siguiente iteración
            }
            // Cada hilo guarda su acarreo final
            data->carries[data->id] = carry_sum; 
        }

        // Sincronización: esperar a que todos terminen la suma
        pthread_barrier_wait(&barrier);

        // Propagación de acarreos: solo el hilo 0 lo hace
        if (*data->is_a_odd && data->id == 0) {
            // Recorre los acarreos de los hilos de derecha a izquierda (desde k-1 hasta 1)
            for (i = data->num_threads - 1; i > 0; i = i - 1) {
                if (data->carries[i] > 0) { 
                    // pos es el dígito justo a la izquierda del inicio del chunk i
                    pos = data->all_starts[i] - 1; 
                    carry_to_propagate = data->carries[i];
                    
                    // Propaga el acarreo hacia la izquierda
                    while (pos >= 0 && carry_to_propagate > 0) {
                        sum = data->result_work[pos] + carry_to_propagate;
                        data->result_work[pos] = sum % 10;
                        carry_to_propagate = sum / 10;
                        pos = pos - 1;
                    }
                    data->carries[i] = 0; // Limpia el acarreo
                }
            }
        }

        // Sincronización: esperar a que el hilo 0 termine de propagar acarreos
        pthread_barrier_wait(&barrier);
        
        // Solo el hilo 0 divide A / 2 (división larga)
        if (data->id == 0) {
            remainder = 0;
            for (i = 0; i < *(data->a_size); i = i + 1) {
                actual = remainder * 10 + data->a_work[i]; // Combina el resto anterior con el dígito actual
                data->a_work[i] = actual / 2;             // Guarda el resultado de la división
                remainder = actual % 2;                   // Guarda el nuevo resto
            }
            
            // Eliminar ceros a la izquierda de A
            while (*(data->a_size) > 1 && data->a_work[0] == 0) {
                // Desplaza todos los dígitos una posición a la izquierda
                for (i = 0; i < *(data->a_size) - 1; i = i + 1) {
                    data->a_work[i] = data->a_work[i+1];
                }
                *(data->a_size) = *(data->a_size) - 1; // Reduce el tamaño de A
            }
        }
        
        // Sincronización: esperar a que el hilo 0 termine de dividir A
        pthread_barrier_wait(&barrier);
        
        // Solo el hilo 0 multiplica B * 2 (multiplicación larga)
        if (data->id == 0) {
            carry_mul = 0;
            // Recorre B de derecha a izquierda
            // Comienza desde el final del arreglo, hasta el dígito más significativo de B
            for (i = data->max_size - 1; i >= *(data->b_start) - 1 && i >= 0; i = i - 1) { 
                temp = data->b_work[i] * 2 + carry_mul;
                data->b_work[i] = temp % 10;
                carry_mul = temp / 10;
            }
            
            // Si B "creció" (hubo un acarreo final), se ajusta el índice de inicio
            if(data->b_work[*(data->b_start)-1] > 0) {
                *(data->b_start) = *(data->b_start) - 1;
            }
        }
        
        // Sincronización: esperar a que el hilo 0 termine de multiplicar B
        pthread_barrier_wait(&barrier);
        
        // Solo el hilo 0 revisa si A llegó a 0
        if (data->id == 0) {
            if (*(data->a_size) == 1 && data->a_work[0] == 0) {
                *data->is_a_zero = 1; // Activa la bandera para salir del bucle
            }
        }
        
        // Sincronización: todos esperan para empezar la siguiente iteración o salir
        pthread_barrier_wait(&barrier);
    }
    
    if (data->opmode == VERBOSE) {
        printf("From %d Ending The Task\n\n", data->id);
    }

    pthread_exit(NULL);
}

void Russian(int k, int mode) {

    int m, n, i;
    clock_t CPU_start, CPU_finish;
    time_t wall_start, wall_finish;
    long wall_time;
    float CPU_time;
    int max_size, a_size, b_size, b_start, start_print;
    int *a_copy, *b_copy, *result; // Arreglos de trabajo
    int *carries, *remainders;     // Arreglos para sincronización
    int is_a_odd, is_a_zero;       // Banderas de estado
    pthread_t *thread;           // Arreglo de hilos
    struct Messages **mess;      // Arreglo de estructuras de mensajes (una por hilo)
    void *exit_status;           // Para pthread_join
    pthread_attr_t attribute;    // Atributos de los hilos
    int s;   // tamaño base del chunk 
    int rem; // resto de chunks 
    int l;   // índice de inicio actual
    
    // Arreglo para guardar los inicios (para el Hilo 0)
    int *offsets; 

    // Leer datos de entrada
    read_data(&m, &n);

    // Configurar tamaños y arreglos de trabajo
    max_size = m + n; // El resultado puede tener hasta m+n dígitos
    a_size = m;
    b_size = n;
    b_start = max_size - b_size; // Índice donde empieza B (alineado a la derecha)

    // Asignar memoria para los arreglos de trabajo
    result = (int *)calloc(max_size, sizeof(int));
    a_copy = (int *)calloc(a_size, sizeof(int));
    b_copy = (int *)calloc(max_size, sizeof(int)); // b_copy tiene max_size para permitir crecimiento
    offsets = (int *)calloc(k, sizeof(int)); // Para guardar los inicios de los chunks

    // Copiar los números leídos a los arreglos de trabajo
    for (i = 0; i < a_size; i = i + 1) {
        a_copy[i] = numbers[0][i];
    }
    for (i = 0; i < b_size; i = i + 1) {
        b_copy[b_start + i] = numbers[1][i]; // Copia B alineado a la derecha
    }
    
    // Liberar la memoria global de lectura, ya no se necesita
    free(numbers[0]);
    free(numbers[1]);
    free(numbers);
    
    // Imprimir datos si estamos en modo verboso
    if (mode == VERBOSE) {
        print_data(a_copy, b_copy, m, n, b_start);
    }

    // Iniciar temporizadores
    wall_start = time(NULL);
    CPU_start = clock();

    // Inicializar banderas y variables de sincronización
    is_a_odd = 0;
    is_a_zero = 0;
    // Caso especial: si A es 0 desde el inicio
    if (a_size == 1 && a_copy[0] == 0) {
        is_a_zero = 1;
    }

    // Inicializar la barrera para k hilos
    pthread_barrier_init(&barrier, NULL, k);

    pthread_attr_init(&attribute);
    thread = (pthread_t *)calloc(k, sizeof(pthread_t));
    pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE); // Hilos "joinables" (esperar por ellos)

    // Asignar memoria para las estructuras de mensajes y sincronización
    mess = (struct Messages **)calloc(k, sizeof(struct Messages *));
    for (i = 0; i < k; i = i + 1)
        mess[i] = (struct Messages *)calloc(1, sizeof(struct Messages));
    carries = (int *)calloc(k, sizeof(int));
    remainders = (int *)calloc(k, sizeof(int));
    
    // Se divide el arreglo result (de max_size) en k chunks
    s = max_size / k;   // Tamaño base del chunk
    rem = max_size % k; // Resto (los primeros 'rem' hilos tendrán un chunk + 1)
    l = 0;              // Índice de inicio acumulado

    //Llenar datos y crear hilos
    for (i = 0; i < k; i = i + 1) {
        
        // Calcular el tamaño de este chunk
        int current_chunk_size;
        if (rem > 0) {
            current_chunk_size = s + 1;
            rem = rem - 1;
        } else {
            current_chunk_size = s;
        }
        
        // Llenar la estructura de mensaje para el hilo i
        offsets[i] = l; // Guardar el inicio de este chunk
        mess[i]->start_index = l;
        mess[i]->end_index = l + current_chunk_size;
        l = l + current_chunk_size; // Actualizar el inicio para el siguiente hilo
        
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
        // Crear el hilo 'i' y ejecutar la función 'Process'
        pthread_create(&thread[i], &attribute, Process, (void *)mess[i]);
    }

    pthread_attr_destroy(&attribute); //Destruir la barrera

    //Esperar a que todos los hilos terminen (join)
    for (i = 0; i < k; i = i + 1) {
        pthread_join(thread[i], &exit_status);
    }

    //Detener temporizadores
    CPU_finish = clock();
    wall_finish = time(NULL);
    wall_time = (long)(wall_finish - wall_start); // Tiempo real (reloj)
    CPU_time = (float)(CPU_finish - CPU_start) / CLOCKS_PER_SEC; // Tiempo de CPU consumido

    //Limpiar barrera
    pthread_barrier_destroy(&barrier);
    
    // Omitir ceros a la izquierda en el resultado
    start_print = 0;
    while (start_print < max_size - 1 && result[start_print] == 0) {
        start_print = start_print + 1;
    }

    if (mode == VERBOSE){
        printf("\n\tResult: ");{
        // Imprimir el resultado dígito por dígito
        for (i = start_print; i < max_size; i = i + 1) {
                printf("%d", result[i]);
            }
        }
    }
    
    //Imprimir tiempos
    printf("\n");
    printf("\nMétodo: Ruso Paralelo (%d hilos)\n", k);
    printf("Tiempo de ejecución CPU (segundos): %f\n", CPU_time);
    printf("Tiempo de ejecución WallTime (segundos): %ld\n", wall_time);

    //Liberar toda la memoria dinámica
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