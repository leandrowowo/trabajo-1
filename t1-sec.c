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

#define SILENT      0
#define VERBOSE     1


 /*
 *
 *
 */

void Usage(char *message) {
  
   printf("\nUsage: %s -M -O < datafile",message);
   printf("\n\nM in {T,R} \t(T: Traditional, R: Russian )");
   printf("\n\nO in {S,V} \t (S: Silence, V: Verbose) \n\n");
}

/*
*
*
*/
void read_data(unsigned int *m, unsigned int *n, unsigned int ***numbers) {
    int i;

    // Leer el numero M indicando cantidad de digitos
    if (scanf("%u", m) != 1) { //Si es diferente de un entero
        printf("\n Error: no se pudo leer el valor de m.\n");
        exit(EXIT_FAILURE);
    }

    *numbers = calloc(2, sizeof(unsigned int *)); //asignar memoria para dos filas
    (*numbers)[0] = calloc(*m, sizeof(unsigned int)); //asignar memoria para "m" columnas de la fila 0

    // Leer dígitos del primer número y verificar que exista esa cantidad
    for (i = 0; i < *m; i = i + 1) {
        if (scanf("%u", &(*numbers)[0][i]) != 1) {
            printf("\n Error: faltan dígitos en la primera fila (se esperaban %u).\n", *m);
            exit(EXIT_FAILURE);
        }
    }

    // Leer el numero N indicando cantidad de digitos
    if (scanf("%u", n) != 1) {
        printf("\nError: no se pudo leer el valor de n.\n");
        exit(EXIT_FAILURE);
    }

    (*numbers)[1] = calloc(*n, sizeof(unsigned int));

    // Leer dígitos del segundo número y verificar que haya "n" digitos
    for (i = 0; i < *n; i = i + 1) {
        if (scanf("%u", &(*numbers)[1][i]) != 1) {
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
void transformar(unsigned int **numbers, unsigned int m, unsigned int n, unsigned long long int *num1, unsigned long long int *num2){
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
//funcion con el metodo de multiplicacion rusa
int russian_secuential(unsigned long long int a, unsigned long long b, float *E_cpu){

    unsigned long long int multiply = 0;
    clock_t start, finish; // Tiempos de CPU

    start = clock();
    while (a >= 1 && b > 0) {
        if (a % 2 != 0) { // Si a es impar
            multiply = multiply + b;
        }
        a = a / 2; // Dividir a entre 2
        b = b * 2; // Multiplicar b por 2
    }
    finish = clock();

    *E_cpu = (float)(finish - start)/CLOCKS_PER_SEC; 

    return multiply;
}


/*
*
*
*/
//funcion de multiplicacion tradicional
unsigned long long traditional_mult(unsigned long long int a, unsigned long long int b, float *E_cpu)
{
    unsigned int digit;
    unsigned long long partial_result ,valor = 0, factor = 1;              // Equivale a las potencias de 10 al momento de multiplicar y correrse un espacio
    clock_t start, finish; // Tiempos de CPU

    start = clock();
    while(b > 0)
    {
        digit = b % 10;                          // Toma el último dígito del segundo factor
        partial_result = a * digit;
        valor = valor + partial_result * factor; //Se simula que el resultado se corre un espacio a la izquierda en base al factor
        b = b / 10;                              // Divide por 10 para tomar el siguiente digito a multiplicar en la próxima iteración
        factor = factor * 10; 
        
    }
    finish = clock();

    *E_cpu = (float)(finish - start)/CLOCKS_PER_SEC; 

    return valor;
}


/*
*
*
*/
void print_data(unsigned int **numbers, unsigned int m, unsigned int n){
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
int main (int argc, char **argv){

    unsigned int m,n, **numbers;           //numero de columnas y el arreglo bidimensional
    unsigned long long int a, b, result;   // resultado de la multiplicación
    int mode;
    float E_cpu, wall_time;
    clock_t start, finish;

    if (argc == 3){

        // Determinar modo
        if(strcmp(argv[2], "-S") == 0)
            mode = SILENT;
        else if(strcmp(argv[2], "-V") == 0)
            mode = VERBOSE;
        else {
            Usage(argv[0]);
            return EXIT_FAILURE;
        }

        start = clock();   // inicia medición de wall time

        read_data(&m, &n, &numbers);
        transformar(numbers, m, n, &a, &b);

        // Ejecutar multiplicación según opción
        if (strcmp(argv[1], "-T") == 0){
            result = traditional_mult(a , b, &E_cpu);
        } else if (strcmp(argv[1], "-R") == 0){
            result = russian_secuential(a , b, &E_cpu);
        } else {
            Usage(argv[0]);
            return EXIT_FAILURE;
        }

        finish = clock();  // fin de wall time
        wall_time = (float)(finish - start) / CLOCKS_PER_SEC;

        // Mostrar resultados
        if (mode == VERBOSE)
            print_data(numbers, m, n);

        printf("\nMultiplication result: %llu\n", result);
        printf("CPU time: %f\n", E_cpu);
        printf("Wall time: %f\n", wall_time);

        // Liberar memoria
        free(numbers[0]);
        free(numbers[1]);
        free(numbers);

    } else {
        Usage(argv[0]);
        return EXIT_FAILURE;
    }
}