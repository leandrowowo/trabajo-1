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

#define SILENT  0
#define VERBOSE 1
#define SECUENTIAL 2
#define PARALLEL    3

/*
 * PROTOTYPES
 */
void read_data(int *m, int *n, int ***numbers);

void imprimir(int m, int n, int **numbers);

void Usage(char *message);


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

void imprimir(int m, int n, int **numbers){
    printf("Primer numero: ");
    for(int i = 0; i < m; i++)
        printf("%d", numbers[0][i]);
    printf("\n");

    printf("Segundo numero: ");
    for(int i = 0; i < n; i++)
        printf("%d", numbers[1][i]);
    printf("\n");
}

//transformar digitos del arreglo en un numero entero


 /*
 * 
 */


int main (int argc, char **argv){

    //metodos de compilacion
    int method, mode;
    int m,n; // tamaño del arreglo
    int **numbers; //arreglo bidimensional
    
    read_data(&m, &n, &numbers);



    if (argc != 4) //Entrada incorrecta
    {
        Usage(argv[0]);
        return 0;
    }
    
    if(strcmp(argv[2],"-T") == 0 && strcmp(argv[3],"-S") == 0){

        //ruso_secuencial();

    }

    else if(strcmp(argv[2],"-P") == 0 && strcmp(argv[3],"-S") == 0){

        //ruso_paralelo();

    }

    else if(strcmp(argv[2],"-T") == 0 && strcmp(argv[3],"-V") == 0){

        imprimir(m, n, numbers);
        //ruso_secuencial();
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