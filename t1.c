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

 /*
 * FUNCTIONS
 */

void Usage(char *message) {
  
   printf("\nUsage: %s k -M -O < datafile",message);
   printf("\n\nk: number of threads (k >= 0)");
   printf("\n\nM in {T,P} \t(T: Secuential, P: Parallel )");
   printf("\n\nO in {S,V} \t (S: Silence, V: Verbose) \n\n");
}

void read_data(int *m, int *n, int ***numbers) {

    //PONER CONDICION DE QUE SI HAY MÁS DE DOS FILAS, ERROR
    //PONER CONDICION DE QUE SI HAY MÁS DE 2 COLUMNAS, ERROR
    //PONER CONDICION DE QUE SI HAY MÁS NUMEROS QUE LA CANTIDAD DE DIGITOS DE M Y N, ERROR

    scanf("%d", m); // cantidad de dígitos del primer número
    *numbers = (int **)calloc(2, sizeof(int *)); // memoria para dos filas

    (*numbers)[0] = (int *)calloc(*m, sizeof(int)); // asignamos memoria para la primera fila cantidad M

    for(int i = 0; i < *m; i = i + 1)
        scanf("%d", &(*numbers)[0][i]); //asignamos los valores a la primera fila

    scanf("%d", n); // cantidad de dígitos del segundo número

    (*numbers)[1] = (int *)calloc(*n, sizeof(int)); // asignamos memoria para la segunda fila cantidad N

    for(int i = 0; i < *n; i = i + 1)
        scanf("%d", &(*numbers)[1][i]); //asignamos los valores a la segunda fila
}

//prueba para imprimir los numeros 
void imprimir(int m, int n, int **numbers){
    printf("\nPrimer numero: ");
    for(int i = 0; i < m; i = i + 1)
        printf("%d", numbers[0][i]); //impresion de la fila 0
    printf("\n");

    printf("\nSegundo numero: ");
    for(int i = 0; i < n; i = i + 1)
        printf("%d", numbers[1][i]); // impresion de la fila 1
    printf("\n");
}

int transformar(int **numbers, int m, int n, int *num1, int *num2){
    *num1 = 0;
    *num2 = 0;

    for(int i = 0; i < m; i = i + 1)
        *num1 = *num1*10 + numbers[0][i]; //transformar la fila 0 en un numero entero

    for(int i = 0; i < n; i = i + 1)
        *num2 = *num2*10 + numbers[1][i]; // lo mismo para este pero con la fila 1
}

int russian_secuential(int *a, int *b){
    int multiply =  0;
    while (*a >= 1 && *b > 0) {
        if (*a % 2 != 0) { // Si a es impar
            multiply = multiply + *b;
        }
        *a = *a / 2; // Dividir a entre 2
        *b = *b * 2; // Multiplicar b por 2
    }
    return multiply;
}


 /*
 * 
 */


int main (int argc, char **argv){

    int m,n; // tamaño del arreglo
    int **numbers; //arreglo bidimensional
    int a, b; // numeros a multiplicar
    int result; // resultado de la multiplicacion
    float E_cpu;
    clock_t csc, cec;

    
    read_data(&m, &n, &numbers);

    transformar(numbers, m, n, &a, &b);

    if (argc != 4) //Entrada incorrecta
    {
        Usage(argv[0]);
        return 0;
    }
    
    if(strcmp(argv[2],"-T") == 0 && strcmp(argv[3],"-S") == 0){

        csc = clock(); // cpu start
        result = russian_secuential(&a, &b);
        cec = clock(); // cpu exit

        E_cpu = (float)(cec - csc) / CLOCKS_PER_SEC;

        printf ("Resultado: %d\n", result);
        printf("CPU time: %f\n", E_cpu);
    }

    else if(strcmp(argv[2],"-P") == 0 && strcmp(argv[3],"-S") == 0){

        //ruso_paralelo();

    }

    else if(strcmp(argv[2],"-T") == 0 && strcmp(argv[3],"-V") == 0){

        imprimir(m, n, numbers);
        result = russian_secuential(&a, &b);
        printf ("\n\nResultado: %d\n", result);
    }

    else if(strcmp(argv[2],"-P") == 0 && strcmp(argv[3],"-V") == 0){

        imprimir(m, n, numbers); //Funcion distinta para ver que le toca a cada hilo
        //ruso_paralelo();
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