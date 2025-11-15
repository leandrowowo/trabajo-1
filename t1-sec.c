/******************************************************************************
 * 
 * FILE: t1.c 
 *
 * DESCRIPTION: Realizar la multiplicación de dos numeros (n y m) con el metodo ruso.
 *             El programa debe ser capaz de correr en dos modos:
 *              - Tradicional.        
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
void read_data(int *m, int *n, int ***numbers)
{
    int i, j;

    *numbers = (int **)calloc(2, sizeof(int *)); // Memoria para dos filas

    scanf("%d", m); // Cantidad de dígitos del primer número
    (*numbers)[0] = (int *)calloc(*m, sizeof(int)); // Asignamos memoria para la primera fila cantidad M

    for(i = 0; i < *m; i = i + 1)
    {
        scanf("%d", &(*numbers)[0][i]); //Asignamos los valores a la primera fila
    }

    scanf("%d", n); // Cantidad de dígitos del segundo número
    (*numbers)[1] = (int *)calloc(*n, sizeof(int)); // Asignamos memoria para la segunda fila cantidad N

    for(j = 0; j < *n; j = j + 1)
    {
        scanf("%d", &(*numbers)[1][j]); //Asignamos los valores a la segunda fila
    }
}


/*
*
*
*/

void russian(int **numbers, int m, int n) {

    clock_t CPU_start, CPU_finish; // Tiempos de CPU
    time_t wall_start, wall_finish; // Tiempos de Wall
    long wall_time;
    float CPU_time;

    int i, carryA, carryB, carry_sum, actual, sum, temp;

    int *a = numbers[0];
    int *b = numbers[1];

    int size_a = m;                     // tamaño actual de A
    int size_b = n;                     // tamaño actual de B
    int max_size = m + n;          // tamaño máximo seguro para result y B (extra para acarreos)

    // Reservamos espacio extra para result y para B
    int *result = (int *)calloc(max_size, sizeof(int));
    int *B_copy = (int *)calloc(max_size, sizeof(int));

    // Copiamos B en B_copy para multiplicarlo sin afectar original
    for (i = 0; i < size_b; i = i + 1) {
        B_copy[i + max_size - size_b] = b[i];
    }

    int start_b = max_size - size_b; // posición inicial de B en el array grande

    wall_start = time(NULL);
    CPU_start = clock();

    while (!(size_a == 1 && a[0] == 0)) {
        carryA = 0;
        carryB = 0;

        // Si A es impar, sumamos B_copy a result
        if (a[size_a - 1] % 2 != 0) {
            carry_sum = 0;
            int pos_result = max_size - 1;
            int pos_b = max_size - 1;

            // Sumar B_copy a result
            while(pos_b >= start_b) {
                sum = result[pos_result] + B_copy[pos_b] + carry_sum;
                result[pos_result] = sum % 10;
                carry_sum = sum / 10;

                pos_result = pos_result - 1;
                pos_b = pos_b - 1;
            }

            // Propagar carry restante
            while(carry_sum > 0 && pos_result >= 0) {
                sum = result[pos_result] + carry_sum;
                result[pos_result] = sum % 10;
                carry_sum = sum / 10;

                pos_result = pos_result - 1;
            }
        }

        // Dividir A entre 2
        for (i = 0; i < size_a; i = i + 1) {
            actual = carryA * 10 + a[i];
            a[i] = actual / 2;
            carryA = actual % 2;
        }

        // Eliminar ceros a la izquierda de A
        while (size_a > 1 && a[0] == 0) {
            for (i = 0; i < size_a - 1; i = i + 1) {
                a[i] = a[i + 1];
            }
            size_a = size_a - 1;
        }

        // Multiplicar B_copy por 2
        for (i = max_size - 1; i >= start_b; i = i - 1) {
            temp = B_copy[i] * 2 + carryB;
            B_copy[i] = temp % 10;
            carryB = temp / 10;
        }

        // Propagar carry al inicio de B_copy
        while (carryB > 0) {
            start_b = start_b - 1;
            B_copy[start_b] = carryB % 10;
            carryB /= 10;
        }
    }

    CPU_finish = clock();
    wall_finish = time(NULL);

    wall_time = (long)(wall_finish - wall_start); // Cálculo de tiempo de procesos (Wall time)
    CPU_time = (float)(CPU_finish - CPU_start) / CLOCKS_PER_SEC; // Cálculo de tiempo de CPU (CPU time)

    //Quitar 0 a la izquierda del resultado
    while (max_size > 1 && result[0] == 0) {
        for (i = 0; i < max_size - 1; i = i + 1) {
            result[i] = result[i + 1];
        }
        max_size = max_size - 1;
    }


    printf("\n\tResult: ");
    for (i = 0; i < max_size; i = i + 1){
        printf("%d", result[i]);
    }
    printf("\n");
    printf("\nTiempo de ejecución CPU (segundos): %f\n", CPU_time);
    printf("Tiempo de ejecución total (segundos): %ld\n", wall_time);
}

/*
*
*
*/

void print_data(int **numbers, int m, int n){
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

    int m,n, **numbers;           //numero de columnas y el arreglo bidimensional
    int mode;

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

        read_data(&m, &n, &numbers);

        
        // Mostrar resultados
        if (mode == VERBOSE){
            print_data(numbers, m, n);
        }

        // Ejecutar multiplicación según opción
        if (strcmp(argv[1], "-T") == 0){
            //traditional_mult(a , b);
        } else if (strcmp(argv[1], "-R") == 0){
            russian(numbers, m, n);
        } else {
            Usage(argv[0]);
            return EXIT_FAILURE;
        }

        // Liberar memoria
        free(numbers[0]);
        free(numbers[1]);
        free(numbers);

    } else {
        Usage(argv[0]);
        return EXIT_FAILURE;
    }
}