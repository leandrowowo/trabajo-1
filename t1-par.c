/******************************************************************************
 * 
 * FILE: t1.c 
 *
 * DESCRIPTION: Realizar la multiplicación de dos numeros (n y m) con el metodo ruso.
 *             El programa debe ser capaz de correr en dos modos:
 *              - Tradicional.        
 *              - Paralelizado (utilizando pthreads).
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
#include <pthread.h>


#define TRADITIONAL 0
#define RUSSIAN     1
#define SILENT      2
#define VERBOSE     3

//trabajaremos con a y b que son los valores que obtienen el valor del arreglo.
//m es la cantidad de digitos del número y así se sabe cuando se debe recorrer el arreglo numbers[0][i]
//lo mismo con n pero para el segundo dígito con numbers[1][i]
struct Message{
    int myid,   //ID del hilo
    size,       //Tamaño del hilo
    opmode,     //modo de operación(Verbose o Silent)
    opmethod,   //método de operación(Tradicional o Ruso)
    size_a,     //tamaño del arreglo a
    carryA,     //acarreo del arreglo a
};


//variable global
int **numbers; //arreglo bidimensional donde esta el datafile
int *a, *b; //arreglo para el primer número y segundo numero



/*
*
*
*/

void *Process(void *p) {
    struct Message *me = (struct Message *)p;
    int i, actual;

    if(me->opmethod == RUSSIAN) {
        while(!(me->size_a == 1 && a[0] == 1)) {

            //DIVISION DE A EN 2 CON EL HILO 0
            me->carryA = 0;
            if(me->myid == 0){ // Solo el hilo 0 divide A entre 2
                for(i = 0; i < me->size_a; i = i + 1) {
                    actual = me->carryA * 10 + a[i];
                    a[i] = actual / 2;
                    me->carryA = actual % 2;

                    if(me->opmode == VERBOSE)
                        printf("Hilo %d, a[%d] = %d, carry = %d\n", me->myid, i, a[i], me->carryA);
                }

                // Eliminar ceros a la izquierda
                while(me->size_a > 1 && a[0] == 0) {
                    for(i = 0; i < me->size_a - 1; i = i + 1)
                        a[i] = a[i + 1];
                    me->size_a = me->size_a - 1;
                }
            }
            if(me->opmode == VERBOSE)
                printf("Hilo %d terminó paso con carry = %d, tamaño actual = %d\n\n", me->myid, me->carryA, me->size_a);
        }

        if(me->opmode == VERBOSE)
            printf("Hilo %d terminó, arreglo A = 1\n", me->myid);
    }

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
    printf("%u\n", m);                 //cantidad de digitos para n
    for (i = 0; i < m; i = i + 1)
    {
        printf("%u\n", numbers[1][i]);  // imprimir digitos par la fila 1
    }
    
}

/*
*
*
*/
void Usage(char *message) {
  
   printf("\nUsage: %s k -M -O < datafile",message);
   printf("\n\nk: Theads Number\n\n");
   printf("\n\nM in {T,R} \t(T: Traditional, R: Russian )");
   printf("\n\nO in {S,V} \t (S: Silence, V: Verbose) \n\n");
}


/*
*
*
*/
int main(int argc, char **argv){

    unsigned int m,n;
    int mode, method, k, s, rem, i;
    struct Message **me;
    pthread_t *thread;
    pthread_attr_t attribute;
    void *exit_status;



    //entrada es ./a.exe k -M -O < datafile.txt
    if(argc == 4){
        if(strcmp(argv[2], "-T") == 0)
            method = TRADITIONAL;
        if(strcmp(argv[2], "-R") == 0)
            method = RUSSIAN;

        if(strcmp(argv[3], "-S") == 0)
            mode = SILENT;
        if(strcmp(argv[3], "-V") == 0)
            mode = VERBOSE;

        k = atoi(argv[1]);  //Obtener cantidad de hebras
        read_data(&m, &n);
        
        if(mode == VERBOSE)
            print_data(m,n);
        
        thread = calloc(k, sizeof(pthread_t));

        a = (int *)calloc(m, sizeof(int));
        for (i = 0; i < m; i = i + 1){
            a[i] = numbers[0][i]; // primer numero
        }

        b = (int *)calloc(m, sizeof(int));
        for (i = 0; i < n; i = i + 1){
            a[i] = numbers[1][i]; // primer numero
        }
        
        me = calloc(k, sizeof(struct Message *));

        for (i = 0; i < k; i = i + 1){
            me[i] = calloc(1, sizeof(struct Message));
        }
        pthread_attr_init(&attribute);
        pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE);
        
        s = m / k;
        rem = m % k;

        // DEFINIR LOS DATOS PARA EL LOS HILOS
        for(i = 0; i < k; i = i + 1){
            if(mode == VERBOSE)
                printf("Main: creating thread %d\n", i);
            me[i]->myid = i;
            if(rem != 0){
                me[i]->size = s + 1;
                rem = rem - 1;
            } 
            else
                me[i]->size = s;
            me[i]->opmode = mode;
            me[i]->opmethod = method;
            me[i]->size_a = m;
            me[i]->carryA = 0;
            me[i]->remainder = 0;

        pthread_create(&thread[i],&attribute,Process,(void *) me[i]);
        }

        pthread_attr_destroy(&attribute);

        for (i = 0; i < k; i = i + 1){
            pthread_join(thread[i],&exit_status);
        }

    } else {
        Usage(argv[0]);
    }

    // Liberar memoria
    free(numbers[0]);
    free(numbers[1]);
    free(numbers);
} 