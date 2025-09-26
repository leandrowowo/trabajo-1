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
    int myid, size, opmode, opmethod, avalue, bvalue, partialresult;
};


//variable global
unsigned int **numbers;


/*
*
*
*/

/*
void Process(void *p){

    struct Message *me; 
    if(me-> opmode == TRADITIONAL){

        

    }

    Hacerlo de manera secuencial parecido al BUSY WAITING
    (!) preguntarle a schiaffino 
    if(me->opmode == RUSSIAN){
        if (me->)
        


    }

}
/*


/*
*
*
*/
void read_data(unsigned int *m, unsigned int *n) {
    int i;

    // Leer el numero M indicando cantidad de dígitos
    if (scanf("%u", m) != 1) {
        printf("\nError: no se pudo leer el valor de m.\n");
        exit(EXIT_FAILURE);
    }

    numbers = calloc(2, sizeof(unsigned int *));   // 2 filas
    numbers[0] = calloc(*m, sizeof(unsigned int)); // fila 0 con m columnas

    for (i = 0; i < *m; i++) {
        if (scanf("%u", &numbers[0][i]) != 1) {
            printf("\nError: faltan dígitos en la primera fila (se esperaban %u).\n", *m);
            exit(EXIT_FAILURE);
        }
    }

    // Leer el numero N indicando cantidad de dígitos
    if (scanf("%u", n) != 1) {
        printf("\nError: no se pudo leer el valor de n.\n");
        exit(EXIT_FAILURE);
    }

    numbers[1] = calloc(*n, sizeof(unsigned int)); // fila 1 con n columnas

    for (i = 0; i < *n; i++) {
        if (scanf("%u", &numbers[1][i]) != 1) {
            printf("\nError: faltan dígitos en el segundo número (se esperaban %u).\n", *n);
            exit(EXIT_FAILURE);
        }
    }
}

/*
*
*
*/
//transformar los digitos del arreglo a numero entero
void transformar(unsigned int m, unsigned int n, unsigned long long int *num1, unsigned long long int *num2){
    int i;
    *num1 = 0;
    *num2 = 0;

    for(i = 0; i < m; i = i + 1)
        *num1 = *num1*10 + numbers[0][i]; // transformar la fila 0 en un número entero

    for(i = 0; i < n; i = i + 1 )
        *num2 = *num2*10 + numbers[1][i]; // transformar la fila 1
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
    int mode, method, k, s, rem, l, i;
    unsigned long long int a, b;
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
        transformar(m,n, &a, &b);
        
        if(mode == VERBOSE)
            print_data(m,n);
        
        thread = calloc(k, sizeof(pthread_t));

        me = calloc(k, sizeof(struct Message *));

        for (i = 0; i < k; i = i + 1){
            me[i] = calloc(1, sizeof(struct Message))
        }
        pthread_attr_init(&attribute);
        pthread_attr_setdetachstate(&attribute,PTHREAD_CREATE_JOINABLE);
    
        s = a / k; //a es el numero que detiene la multiplicacion rusa
        rem = a % k;
        l = 0;

        //int myid, size, opmode, method, avalue, bvalue, partialresult;
        for(mode == VERBOSE){
            if (mode == VERBOSE)
                printf("Main: creating thread %d\n", i);
            m[i]->myid = i;
	    if (rem != 0) {
	        m[i]->size = s + 1;
	        rem = rem - 1;
	    }
	    else
	        m[i]->size = s;
        m[i]->opmode = mode;
        m[i]->opmethod = method;
        m[i]->avalue = a;
        m[i]->bvalue = b;
        m[i]->partialresult = 0;
        l = l + m[i]->size;
        pthread_create(&thread[i],&attribute,Process,(void *) m[i]);
        }

        pthread_attr_destroy(&attribute);

        for (i = 0; i < k; i = i + 1){
            pthread_join(thread[i],&exit_status);
        }

    } else {
        Usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Liberar memoria
    free(numbers[0]);
    free(numbers[1]);
    free(numbers);

}
