/******************************************************************************
 * 
 * FILE: t1.c 
 *
 * DESCRIPTION: Realizar la multiplicación de dos numeros (n y m) con el metodo ruso.
 *             El programa debe ser capaz de correr en dos modos:
 *              - Tradicional.        
 *              - Paralelizado (utilizando pthreads).
 * 
 * AUTHOR: Delian Santis López
 *
 * 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SILENT  0
#define VERBOSE 1
#define TRADITIONAL 2
#define PARALLEL    3

/*
 * PROTOTYPES
 */



 /*
 * FUNCTIONS
 */

unsigned long russian_multiplication(){

//PENDIENTE HACER UN READ DATA Y LLENAR CON DATOS LOS ARREGLOS 

}


 /*
 * 
 */
void Usage(char *message) {
  
   printf("\nUsage: %s k -M -O < datafile",message);
   printf("\n\nk: number of threads (k >= 0)");
   printf("\n\nM in {T,P} \t(T: Traditional, P: Parallel )");
   printf("\n\nO in {S,V} \t (S: Silence, V: Verbose) \n\n");
}


int main (int argc, char **argv){

    //metodos de compilacion
    int method, mode;
    //
    int m,n; // tamaño del arreglo
    int *numb1 = NULL, *numb2 = NULL; //arreglos para guardar los numeros



    if (argc < 4) //Entrada incorrecta
    {
        Usage(argv[0]);
        return 0;
    }


    if (argc == 3){ //Entrada k -M -O < datafile.txt
    
        if (strcmp(argv[2],"-T") == 0)
            method = TRADITIONAL;
        
        else if(strcmp(argv[2],"-P") == 0)
            method = PARALLEL;
        else{
            Usage(argv[0]);
            return 0;
        }

        //HACER LLAMADA PARA LLENAR LOS ARREGLOS.
        //read_data(&numb1, &numb2, &m, &n); //Pendiente hacer la funcion read_data


        if (strcmp(argv[1],"-S") == 0)
            mode = SILENT;
        if (strcmp(argv[1],"-V") == 0)
            mode = VERBOSE;


        if (method == TRADITIONAL)
            russian_multiplication();
        else if(method == PARALLEL)
            russian_multiplication(); //Pendiente metodo paralelo
    }
}
