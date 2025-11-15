/******************************************************************************
 * * FILE: t1.c 
 *
 * DESCRIPTION: Realizar la multiplicación de dos numeros (n y m) con el metodo ruso.
 * (Versión Pthreads con Busy-Waiting, estilo parsummutex.c)
 * * AUTHOR: Delian Santis López - Leandro Aballay Henriquez
 * * ENTREGA: 06/10/2025
 * *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>


#define TRADITIONAL 0
#define RUSSIAN     1
#define SILENT      2
#define VERBOSE     3

// --- Estructura de Mensaje (Adaptada de parsummutex.c) ---
struct Message{
    int myid;       //ID del hilo
    int opmode;     //modo de operación(Verbose o Silent)
    int opmethod;   //método de operación(Tradicional o Ruso)
    int hmthreads;  // 'k' (how many threads)

    // Información específica de nuestra tarea
    int size_a_initial;     // m
    int size_b_initial;     // n
    int size_result_initial; // m+n
    int start_b_initial; // Posición inicial de B
};


// --- Variables Globales (como en parsummutex.c) ---
int **numbers; 
int *a, *b;     
int *result;

// Tiempos globales
clock_t CPU_start, CPU_finish;
time_t wall_start, wall_finish;

// --- Sincronización (Lógica de parsummutex.c) ---
pthread_mutex_t lock;       // Mutex global (Requerido por parsummutex.c)
unsigned char flag;         // Flag global para turnos (Modo 1 y 2)

// Estado global del bucle (controlado por Hilo 0)
int g_loop_active = 1;      // 1 = bucle sigue, 0 = terminar
int g_A_is_odd = 0;         // Orden de Hilo 0 para Hilo 1
int g_size_a;               // Tamaño actual de A
int g_start_b;              // Posición inicial actual de B
int g_max_size;             // Tamaño de result y B (m+n)


/*
*
*
*/

void *Process(void *p){
    struct Message *m = (struct Message *) p;
    int myid = m->myid;
    int k = m->hmthreads;

    // Variables locales para los cálculos
    int i, carry, actual, sum, temp;

    if (myid == 0) {
        // Hilo 0 (Maestro) inicializa los valores globales
        g_size_a = m->size_a_initial;
        g_start_b = m->start_b_initial;
        g_max_size = m->size_result_initial;
        if(m->opmode == VERBOSE)
            printf("Thread %d (Maestro): Iniciando.\n", myid);
    } else {
        if(m->opmode == VERBOSE)
            printf("Thread %d (Esclavo): Iniciando.\n", myid);
    }

    // --- Lógica del MAESTRO (Hilo 0) ---
    if (myid == 0) {
        
        while(g_loop_active) {
            // 1. Esperar mi turno
            while (flag != 0) { /* spin */ }

            // 2. TAREA 0: Operaciones de A
            g_A_is_odd = (a[g_size_a - 1] % 2 != 0);

            carry = 0;
            for (i = 0; i < g_size_a; i = i + 1) {
                actual = carry * 10 + a[i];
                a[i] = actual / 2;
                carry = actual % 2;
            }

            while (g_size_a > 1 && a[0] == 0) {
                for (i = 0; i < g_size_a - 1; i = i + 1) {
                    a[i] = a[i + 1];
                }
                g_size_a = g_size_a - 1;
            }
            
            // 3. Revisar si terminamos
            if (g_size_a == 1 && a[0] == 0) {
                g_loop_active = 0; // Poner señal de parada
            }
            
            // 4. Pasar el turno
            flag = (flag + 1) % k;
        }

        // Hilo 0 terminó. Despertar al siguiente para que salga.
        while (flag != 0) { /* spin (espera si k<3) */ }
        flag = (flag + 1) % k;

    } 
    // --- Lógica del ESCLAVO (Hilo 1...k-1) ---
    else {
        
        while(1) { // Bucle "infinito"
            
            // 1. Esperar mi turno
            while (flag != myid) { /* spin */ }
            
            // Hilo 0 ya corrió (puso g_A_is_odd y g_loop_active)
            
            // --- INICIO DE LA CORRECCIÓN ---
            // 2. Ejecutar TAREAS ANTES de revisar si salimos

            // TAREA 1: Result = Result + B
            if (myid == 1) { 
                if (g_A_is_odd) { // Revisa la orden de Hilo 0
                    if(m->opmode == VERBOSE)
                        printf("Thread %d: Ejecutando R = R + B\n", myid);
                    
                    carry = 0;
                    int pos_result = g_max_size - 1;
                    int pos_b = g_max_size - 1; 

                    while(pos_b >= g_start_b) {
                        sum = result[pos_result] + b[pos_b] + carry;
                        result[pos_result] = sum % 10;
                        carry = sum / 10;
                        pos_result = pos_result - 1;
                        pos_b = pos_b - 1;
                    }
                    while(carry > 0 && pos_result >= 0) {
                        sum = result[pos_result] + carry;
                        result[pos_result] = sum % 10;
                        carry = sum / 10;
                        pos_result = pos_result - 1;
                    }
                }
            } 
            
            // TAREA 2: B = B * 2
            else if (myid == 2) {
                if(m->opmode == VERBOSE)
                    printf("Thread %d: Ejecutando B = B * 2\n", myid);
                
                carry = 0;
                for (i = g_max_size - 1; i >= g_start_b; i = i - 1) {
                    temp = b[i] * 2 + carry;
                    b[i] = temp % 10;
                    carry = temp / 10;
                }
                while (carry > 0) {
                    g_start_b = g_start_b - 1;
                    b[g_start_b] = carry % 10;
                    carry /= 10;
                }
            } 
            // (Hilos 3...k-1 no hacen nada)
            

            // 3. AHORA, revisar si el Maestro nos dijo que saliéramos
            if (!g_loop_active) {
                flag = (flag + 1) % k; // Despertar al siguiente para que salga
                break; // Salir del bucle
            }
            // --- FIN DE LA CORRECCIÓN ---

            // 4. Pasar turno (si no salimos)
            flag = (flag + 1) % k;
        }
    }

    if(m->opmode == VERBOSE)
        printf("Thread %d: Terminando.\n", myid);
    
    pthread_exit(NULL);
}


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

    for(i = 0; i < *m; i = i + 1){
        scanf("%d", &(numbers)[0][i]); //Asignamos los valores a la primera fila
    }

    scanf("%d", n); // Cantidad de dígitos del segundo número
    numbers[1] = (int *)calloc(*n, sizeof(int)); // Asignamos memoria para la segunda fila cantidad N

    for(j = 0; j < *n; j = j + 1){
        scanf("%d", &(numbers)[1][j]); //Asignamos los valores a la segunda fila
    }
}

/*
*
*
*/
void print_data(unsigned int m, unsigned int n){
    int i;

    printf("****** Archivo utilizado ******\n");
    printf("%u\n", m);                 //cantidad de digitos para m
    for (i = 0; i < m; i = i + 1)
    {
        printf("%u\n", numbers[0][i]);  //imprimir digitos para la fila 0
    }
    printf("%u\n", n);                 //cantidad de digitos para n (CORREGIDO)
    for (i = 0; i < n; i = i + 1)
    {
        printf("%u\n", numbers[1][i]);  // imprimir digitos par la fila 1 (CORREGIDO)
    }
    
}

/*
*
*
*/
void print_result(int *res, int size) {
    int i, start = 0;
    
    // Saltar ceros a la izquierda
    while(start < size - 1 && res[start] == 0)
        start++;

    for(i = start; i < size; i++)
        printf("%d", res[i]);
    printf("\n");
}

/*
*
*
*/
void Usage(char *message) {
 
   printf("\nUsage: %s k -M -O < datafile",message);
   printf("\n\nk: Theads Number (se recomienda k >= 3 para paralelismo Ruso)");
   printf("\n\nM in {T,R} \t(T: Traditional, R: Russian )");
   printf("\n\nO in {S,V} \t (S: Silence, V: Verbose) \n\n");
}


/*
*
*
*/
int main(int argc, char **argv){

    unsigned int m, n, sResult;
    int mode = SILENT;
    int method = RUSSIAN;
    int k, i;
    int start_b_initial;
    struct Message **mess; // 'mess' como en parsummutex.c
    pthread_t *thread;
    pthread_attr_t attribute;
    void *exit_status;

    // Tiempos (declarados como en parsummutex.c)
    float E_cpu;
    long E_wall;
    time_t  ts, te;
    clock_t cs, ce;
    
    if(argc == 4){
        
        if(strcmp(argv[2], "-T") == 0)
            method = TRADITIONAL;
        else if(strcmp(argv[2], "-R") == 0)
            method = RUSSIAN;
        else {
            Usage(argv[0]);
            return EXIT_FAILURE;
        }
            
        if(strcmp(argv[3], "-S") == 0)
            mode = SILENT;
        else if(strcmp(argv[3], "-V") == 0)
            mode = VERBOSE;
        else {
            Usage(argv[0]);
            return EXIT_FAILURE;
        }

        k = atoi(argv[1]);
        if (k <= 0) {
             printf("Error: El número de hebras (k) debe ser al menos 1.\n");
             Usage(argv[0]);
             return EXIT_FAILURE;
        }
        if (k < 3 && method == RUSSIAN) {
            printf("Advertencia: Se recomiendan k >= 3. Con k=%d, Hilo %d hará múltiples tareas.\n", k, k-1);
        }

        read_data(&m, &n);
        
        if(mode == VERBOSE)
            print_data(m,n);
        
        // Iniciar Tiempos (como en parsummutex.c)
        ts = time(NULL);
        cs = clock();

        // --- Inicializar Mutex (como en parsummutex.c Modo 3) ---
        // Aunque el busy-wait no lo usa para bloquear, 
        // es buena práctica tenerlo si el flag se modifica (aunque aquí no se usa).
        pthread_mutex_init(&lock, NULL);

        thread = calloc(k, sizeof(pthread_t));
        pthread_attr_init(&attribute);
        pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE);

        // --- Asignación de 'a', 'b' y 'result' (Globales) ---
        a = (int *)calloc(m, sizeof(int));
        for (i = 0; i < m; i = i + 1){
            a[i] = numbers[0][i];
        }
        sResult = m + n; 
        b = (int *)calloc(sResult, sizeof(int));
        result = (int *)calloc(sResult, sizeof(int));
        start_b_initial = sResult - n;
        for (i = 0; i < n; i = i + 1) {
            b[start_b_initial + i] = numbers[1][i];
        }
        
        // --- Lógica de 'mess' (como en parsummutex.c) ---
        mess = calloc(k,sizeof(struct Message *));
        for (i = 0; i < k; i = i + 1)
            mess[i] = calloc(1,sizeof(struct Message));

        flag = 0; // Inicializar flag (como en parsummutex.c)

        
        for(i = 0; i < k; i = i + 1){

            if(mode == VERBOSE)
                printf("Main: creating thread %d\n", i);
            
            // --- Llenado de 'mess' (campos de parsummutex.c) ---
            mess[i]->myid = i;
            mess[i]->opmode = mode;
            mess[i]->hmthreads = k;
            
            // Campos de nuestro problema
            mess[i]->opmethod = method;
            mess[i]->size_a_initial = m;
            mess[i]->size_b_initial = n;
            mess[i]->size_result_initial = sResult;
            mess[i]->start_b_initial = start_b_initial;
        
            pthread_create(&thread[i],&attribute,Process,(void *) mess[i]);
        }

        pthread_attr_destroy(&attribute); // Destruir atributo después de crear

        for (i = 0; i < k; i = i + 1){
            pthread_join(thread[i],&exit_status);
        }
        
        // Detener Tiempos (como en parsummutex.c)
        ce = clock();
        te = time(NULL);
        E_wall = (long) (te - ts);
        E_cpu = (float)(ce - cs) / CLOCKS_PER_SEC;

        // --- Destruir Mutex (como en parsummutex.c) ---
        pthread_mutex_destroy(&lock);

        if(mode == VERBOSE)
            printf("Main: All threads completed\n");
        
        printf("\n\tResult: ");
        print_result(result, sResult);
        printf("\n");

        printf("\nTiempo de ejecución CPU (segundos): %f\n", E_cpu);
        printf("Tiempo de ejecución total (segundos): %ld\n", E_wall);

    } else {
        Usage(argv[0]);
        return EXIT_FAILURE; 
    }

    // Liberar memoria (adaptado)
    for(i = 0; i < k; i = i + 1) 
        free(mess[i]);
    free(mess);
    free(thread);
    free(a);
    free(b);
    free(result);
    free(numbers[0]);
    free(numbers[1]);
    free(numbers);
    
    return EXIT_SUCCESS; 
}