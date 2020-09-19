//
//  main.c
//  Actividad 5- IPC
//
//  Created by Carla Pérez Gavilán Del Castillo on 18/09/2020.
//  Copyright (c) 2020 Carla Pérez Gavilán Del Castillo . All rights reserved.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



    

typedef struct{
    int tuberia[2];
}tube;

int main(int argc, char * const * argv)
{
    char *nvalue = NULL;
    int n = 0;
    int option;
    opterr = 0;
    
    while ((option = getopt (argc, argv, "n:")) != -1)
        switch (option)
    {
        case 'n':
            nvalue = optarg;
            n = atoi(nvalue);
            break;
        case '?':
            if (optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            printf("ERROR: no ingresó la cantidad de procesos que desea o se generarrán 0 procesos\n");
            exit(0);
    
    }
    //Vector de pipes 
    tube * pipeArray;
    tube * last;
    tube * aux;

    //Vector de procesos
    pid_t pid;

    pipeArray = (tube *) malloc(sizeof(tube)*n);
    last = pipeArray + (n);
    int count = 0;

    if(n == 0){
        printf("ERROR: no ingresó la cantidad de procesos que desea o se generarrán 0 procesos\n");
        exit(0);
    }
    
    for(aux = pipeArray; aux<last; aux++){
        pipe(aux->tuberia);
        printf("Inicio de pipe  %d \n", count);
        count++;
    }

    
    char f = 'T';
    write(pipeArray->tuberia[1], &f, sizeof(char));
    printf("—-> Soy el proceso padre inicializando el testigo %c \n", f);

    
    for(int i = 1; i<=n; i++){
        pid = fork();

    if (pid == -1)
    {
        printf("Error al crear el proceso hijo");
        exit(-1);
    }
    else if (pid == 0)
    {
        printf("(%d)--> --> Creación de hijo %d de pid %d \n",i, i, pid);
        char f;
        while(1){
            close((pipeArray+i-1)->tuberia[1]);
            read((pipeArray+i-1)->tuberia[0], &f, sizeof(char));
            printf("PROCESS %d READING FROM %d \n", i, i-1);
            printf("(%d) —-> Soy el proceso número %d con PID %d  y recibí el testigo %c, el cual tendré por 5 segundos \n", i,  i, getpid(), f);
            
            sleep(5); 

            printf("(%d) <—- Soy el proceso número %d con PID %d y acabo de enviar el testigo %c \n",i,  i, getpid(), f);
            if(i==n){
            close((pipeArray)->tuberia[0]);
            write((pipeArray)->tuberia[1], &f, sizeof(char));
            }else{
            close((pipeArray+i)->tuberia[0]);
            write((pipeArray+i)->tuberia[1], &f, sizeof(char));
            }
        }
        exit(0);
    }else{
                    printf("iteración %d \n", i);
    }
     }
        while(1){
            close((pipeArray+n-1)->tuberia[1]);
            read((pipeArray+n-1)->tuberia[0], &f, sizeof(char));
            printf("—-> Soy el proceso PADRE con PID %d  y recibí el testigo %c, el cual tendré por 5 segundos \n", getpid(), f);
            
            sleep(5); 
            
            printf("<—- Soy el proceso PADRE con PID %d y acabo de enviar el testigo %c \n", getpid(), f);
            close((pipeArray)->tuberia[0]);
            write((pipeArray)->tuberia[1], &f, sizeof(char));
        }

        for(aux = pipeArray; aux<last; aux++){
            free(aux);
        }
        free(pipeArray);
   
    
    return 0;
}