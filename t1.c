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

#define VERBOSE 1
#define SILENCE 0

 /*
 * FUNCTIONS
 */

void Usage(char *message) {
  
   printf("\nUsage: %s k -M -O < datafile",message);
   printf("\n\nk: number of threads (k >= 0)");
   printf("\n\nM in {T,R} \t(T: Traditional, R: Russian )");
   printf("\n\nO in {S,V} \t (S: Silence, V: Verbose) \n\n");
}

void read_data(unsigned int *m, unsigned int *n, unsigned int ***numbers) {
    unsigned int i;

    scanf("%d", m); // cantidad de dígitos del primer número
    if (*m > 9){
        exit(EXIT_SUCCESS);
    }
    
    *numbers = (unsigned int **)calloc(2, sizeof(unsigned int *)); // memoria para dos filas

    (*numbers)[0] = (unsigned int *)calloc(*m, sizeof(unsigned int)); // asignamos memoria para la primera fila cantidad M

    for(i = 0; i < *m; i = i + 1)
        scanf("%d", &(*numbers)[0][i]); //asignamos los valores a la primera fila
    
    scanf("%d", n); // cantidad de dígitos del segundo número
    if (*n > 9){
        exit(EXIT_SUCCESS);
    }
    (*numbers)[1] = (unsigned int *)calloc(*n, sizeof(unsigned int)); // asignamos memoria para la segunda fila cantidad N

    for(i = 0; i < *n; i = i + 1)
        scanf("%d", &(*numbers)[1][i]); //asignamos los valores a la segunda fila
}

//prueba para imprimir los numeros 
void imprimir(unsigned int m, unsigned int n, unsigned int **numbers){
    unsigned int i;
    printf("\nPrimer numero: ");
    for(i = 0; i < m; i = i + 1)
        printf("%d", numbers[0][i]); //impresion de la fila 0
    printf("\n");

    printf("\nSegundo numero: ");
    for(i = 0; i < n; i = i + 1)
        printf("%d", numbers[1][i]); // impresion de la fila 1
    printf("\n");
}

void transformar(unsigned int **numbers, unsigned int m, unsigned int n, unsigned long long int *num1, unsigned long long int *num2){
    *num1 = 0;
    *num2 = 0;
    unsigned int i;

    for(i = 0; i < m; i++)
        *num1 = *num1*10 + numbers[0][i]; // transformar la fila 0 en un número entero

    for(i = 0; i < n; i++)
        *num2 = *num2*10 + numbers[1][i]; // transformar la fila 1
}

int russian_secuential(unsigned long long int *a, unsigned long long *b, unsigned int mode){

    unsigned long long int multiply = 0;

    if (mode == 1) {
        printf("Proceso de datos:\n");  // Solo si es verboso
        printf("a \t b\n");         // Mostrar cada numero
        printf("------------ \n");
    }

    while (*a >= 1 && *b > 0) {

        if(mode == 1)
            printf("%llu \t %llu\n", *a, *b);

        if (*a % 2 != 0) { // Si a es impar
            multiply = multiply + *b;
        }
        *a = *a / 2; // Dividir a entre 2
        *b = *b * 2; // Multiplicar b por 2
    }
    return multiply;
}

unsigned long long traditional_mult(unsigned long long int a, unsigned long long int b)
{
    unsigned int digit;
    unsigned long long partial_result;
    unsigned long long valor = 0;
    unsigned long long factor = 1; // Equivale a las potencias de 10 al momento de multiplicar y correrse un espacio

    while(b > 0)
    {
        digit = b % 10;     // Toma el último dígito del segundo factor
        partial_result = a * digit;     // 'a' se convierte a unsigned long long para evitar overflow en la multiplicación
        valor = valor + partial_result * factor;
        b = b / 10;     // Divide por 10 para tomar el siguiente digito a multiplicar en la próxima iteración
        factor = factor * 10;
    }

    return valor;
}


 /*
 * 
 */


int main (int argc, char **argv){

    unsigned int m,n, mode; // tamaño del arreglo y el metodo
    unsigned int **numbers; //arreglo bidimensional
    unsigned long long int a, b;        // números a multiplicar
    unsigned long long int result;       // resultado de la multiplicación
    float E_cpu;
    clock_t csc, cec;

    
    read_data(&m, &n, &numbers);

    transformar(numbers, m, n, &a, &b);

    if (argc != 4) //Entrada incorrecta
    {
        Usage(argv[0]);
        return 0;
    }
    
    if(strcmp(argv[2], "-T") == 0){
        if (strcmp(argv[3], "-S") == 0){
            mode = SILENCE;

            csc = clock(); // cpu start
            result = traditional_mult(a , b);
            cec = clock(); // cpu exit

            E_cpu = (float)(cec - csc) / CLOCKS_PER_SEC;

            printf("Resultado multiplicación: %llu\n", result);
            printf("\nCPU time: %f\n\n", E_cpu);
        }
        else if(strcmp(argv[3], "-V") == 0){
            mode = VERBOSE;
            
            csc = clock(); // cpu start
            result = traditional_mult(a , b);
            cec = clock(); // cpu exit

            E_cpu = (float)(cec - csc) / CLOCKS_PER_SEC;

            printf("Resultado multiplicación: %llu\n", result);
            printf("\nCPU time: %f\n\n", E_cpu);

        }
        else{
            Usage(argv[0]);
            return 0;
        }
    }
    else if (strcmp(argv[2], "-R") == 0)
    {
        if (strcmp(argv[3], "-S") == 0)
        {
            mode = SILENCE;
            csc = clock(); // cpu start
            result = russian_secuential(&a, &b, mode);
            cec = clock(); // cpu exit

            E_cpu = (float)(cec - csc) / CLOCKS_PER_SEC;

            printf ("\nResultado: %llu\n", result);
            printf("\nCPU time: %f\n\n", E_cpu);
        }

        else if(strcmp(argv[3], "-V") == 0){
            mode = VERBOSE;
            csc = clock() ;
            result = russian_secuential(&a, &b, mode);
            cec = clock(); // cpu exit

            E_cpu = (float)(cec - csc) / CLOCKS_PER_SEC;

            printf ("\nResultado: %llu\n", result);
            printf("\nCPU time: %f\n", E_cpu);
        }
        else{
            Usage(argv[0]);
            return 0;
        }
    }
    else{
        Usage(argv[0]);
        return 0;
    }

    free(numbers[0]);
    free(numbers[1]);
    free(numbers);

    return 0;
}