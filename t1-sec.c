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

    int i, k; //Indices y contadores de los bucles
    int carryA, carryB, carry_sum; // Varuiables para el acarreo 
    int prod, temp, actual, sum; //Variables auxiliares para las operaciones

    int *a = numbers[0]; // fila 0
    int *b = numbers[1]; // fila 1

    int size = m; //Tamaño del arreglo A y luego ir reduciendo si hay 0

    //Arreglo para almacenar el resultado si A es impar
    int max_size = m + n; //Cantidad máxima de dígitos del resultado
    int *result = (int *)calloc(max_size, sizeof(int));


    // Mientras el número no sea 1
    while (!(size == 1 && a[0] == 0)) {

        carryA = 0; //Se reinicia el carry de A
        carryB = 0; //Se reinicia el carry de B

        if(a[size - 1] % 2 != 0){
            
            carry_sum = 0;  // Reiniciar carry para la suma
            k = max_size - n;

            for (i = n - 1; i >= 0; i = i - 1) {
                sum = result[k + i] + b[i] + carry_sum;
                result[k + i] = sum % 10;
                carry_sum = sum / 10;
            }

            // Propagar carry si sobra
            while(carry_sum > 0 && k > 0){
                k = k - 1;
                sum = result[k] + carry_sum;
                result[k] = sum % 10;
                carry_sum = sum / 10;
            }
        }

        /*
        *
        */
        //Dividir el arreglo A
        for (i = 0; i < size; i = i + 1) {
            actual = carryA * 10 + a[i];
            a[i] = actual / 2;
            carryA = actual % 2;
        }

        // Eliminar ceros a la izquierda
        while (size > 1 && a[0] == 0) {
            for (i = 0; i < size - 1; i = i + 1) {
                a[i] = a[i + 1];
            }
            size = size - 1;
        }
        /*
        *
        */
        //Multiplicar arreglo B
        // Recorremos de derecha a izquierda
        for(i = n - 1; i >= 0; i = i - 1)
        {
            temp = b[i] * 2 + carryB;
            b[i] = temp % 10;
            carryB = temp / 10;
        }

        // Si después de procesar todos los dígitos aún queda un acarreo
        if(carryB > 0)
        {
            // Desplazamos a la derecha
            for(i = n; i > 0; i = i - 1)
            {
                b[i] = b[i-1];
            }
            b[0] = carryB;
            n = n + 1; // Aumentamos la longitud del número
        }
        /*
        *
        */
       /*

        // Debug: mostrar paso
        printf("A: ");
        for (i = 0; i < size; i = i + 1) 
            printf("%d", a[i]);

        printf(" | B: ");
        for (i = 0; i < n; i = i + 1) 
            printf("%d", b[i]);

        printf("\tResult: ");
        for (i = 0; i < max_size; i = i + 1) 
            printf("%d", result[i]);
        printf("\n\n");
        */
    }

    if (a[0] == 1) {

        carry_sum = 0;
        k = max_size - n;

        for (i = n - 1; i >= 0; i = i - 1) {
            sum = result[k + i] + b[i] + carry_sum;
            result[k + i] = sum % 10;
            carry_sum = sum / 10;
        }
        while(carry_sum > 0 && k > 0) {
            sum = result[k] + carry_sum;
            result[k] = sum % 10;
            carry_sum = sum / 10;
            k = k - 1;
        }
    }
    
    // Eliminar ceros a la izquierda
    while (max_size > 1 && result[0] == 0) {
        for (i = 0; i < max_size - 1; i = i + 1) {
            result[i] = result[i + 1];
        }
        max_size = max_size - 1;
    }

    printf("\tResult: ");
    for (i = 0; i < max_size; i = i + 1) 
        printf("%d", result[i]);
    printf("\n\n");
    

    free(result);
}


/*
//funcion con el metodo de multiplicacion rusa
void russian_secuential(unsigned long long int a, unsigned long long b){

    unsigned long long int multiply = 0;
    long E_wall;
    float E_cpu;
    clock_t start, finish; // Tiempos de CPU
    time_t  ts, te;

    ts = time(NULL);
    start = clock();

    start = clock();
    while (a >= 1 && b > 0) {
        if (a % 2 != 0) { // Si a es impar
            multiply = multiply + b;
        }
        a = a / 2; // Dividir a entre 2
        b = b * 2; // Multiplicar b por 2
    }
    finish = clock();
    te = time(NULL);

    E_wall = (long) (te - ts);
    E_cpu = (float)(finish - start)/CLOCKS_PER_SEC;

    printf("\nMultiplication result: %llu\n", multiply);
    printf("CPU time: %f\n", E_cpu);
    printf("Wall time: %f\n", E_wall);

}


/*
*
*
*/
//funcion de multiplicacion tradicional
/*
void traditional_mult(unsigned long long int a, unsigned long long int b)
{
    unsigned int digit;
    long E_wall;
    float E_cpu;
    unsigned long long partial_result ,valor = 0, factor = 1;              // Equivale a las potencias de 10 al momento de multiplicar y correrse un espacio
    clock_t start, finish; // Tiempos de CPU
    time_t  ts, te;

    ts = time(NULL);
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
    te = time(NULL);

    E_wall = (long) (te - ts);
    E_cpu = (float)(finish - start)/CLOCKS_PER_SEC;

    printf("\nMultiplication result: %llu\n", valor);
    printf("CPU time: %f\n", E_cpu);
    printf("Wall time: %f\n", E_wall);
}


/*
*
*
*/
/*
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
            //print_data(numbers, m, n);
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