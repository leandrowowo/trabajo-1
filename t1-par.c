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
struct Message{
    int myid, size, opmode, avalue, bvalue;
};


unsigned int **numbers;


/*
*
*
*/
//void Process(void *p){





//}



/*
*
*
*/
void read_data(int m, int n) {
    int i;

    // Leer el numero M indicando cantidad de digitos
    if (scanf("%d", &m) != 1) { //Si es diferente de un entero
        printf("\n Error: no se pudo leer el valor de m.\n");
        exit(EXIT_FAILURE);
    }

    numbers = calloc(2, sizeof(unsigned int *)); //asignar memoria para dos filas
    (numbers)[0] = calloc(m, sizeof(unsigned int)); //asignar memoria para "m" columnas de la fila 0

    // Leer dígitos del primer número y verificar que exista esa cantidad
    for (i = 0; i < m; i = i + 1) {
        if (scanf("%d", &(numbers)[0][i]) != 1) {
            printf("\n Error: faltan dígitos en la primera fila (se esperaban %d).\n", m);
            exit(EXIT_FAILURE);
        }
    }

    // Leer el numero N indicando cantidad de digitos
    if (scanf("%d", &n) != 1) {
        printf("\nError: no se pudo leer el valor de n.\n");
        exit(EXIT_FAILURE);
    }

    (numbers)[1] = calloc(n, sizeof(unsigned int));

    // Leer dígitos del segundo número y verificar que haya "n" digitos
    for (i = 0; i < n; i = i + 1) {
        if (scanf("%u", &(numbers)[1][i]) != 1) {
            printf("\nError: faltan dígitos en el segundo número (se esperaban %u).\n", n);
            exit(EXIT_FAILURE);
        }
    }

    //Agregado al final para mostrar lo leído
    printf("\n--- Datos leídos ---\n");
    printf("Cantidad de dígitos primer número (m): %d\n", m);
    printf("Primer número (dígitos): ");
    for (i = 0; i < m; i++) {
        printf("%u ", numbers[0][i]);
    }
    printf("\n");

    printf("Cantidad de dígitos segundo número (n): %d\n", n);
    printf("Segundo número (dígitos): ");
    for (i = 0; i < n; i++) {
        printf("%u ", numbers[1][i]);
    }
    printf("\n---------------------\n");

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

    int mode, method, k, m, n;
    struct Message **me;
    pthread_t *thread;
    pthread_attr_t attribute;

    //entrada es ./a.exe k -M -O < datafile.txt
    if(argc == 4){
        if(strcmp(argv[3], "-S") == 0)
            mode = SILENT;
        if(strcmp(argv[3], "-V") == 0)
            mode = VERBOSE;

        k = atoi(argv[1]);  //Obtener cantidad de hebras
        read_data(m, n);
        


        if(strcmp(argv[2], "-T") == 0)
            method = TRADITIONAL;
        if(strcmp(argv[2], "-R") == 0)
            method = RUSSIAN;

    } else {
        Usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Liberar memoria
    free(numbers[0]);
    free(numbers[1]);
    free(numbers);

}
