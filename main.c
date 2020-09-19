//
//  main.c
//  Actividad 5- IPC
//
//  Created by Carla Pérez Gavilán Del Castillo on 18/09/2020.
//  Copyright (c) 2020 Carla Pérez Gavilán Del Castillo . All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


typedef struct{
    int tuberia[2];
}tube;

int main(int argc, const char * argv[])
{
    //Vector de pipes 
    tube * pipeArray;
    tube * last;
    tube * aux;

    //Vector de procesos
    pid_t pid;
    int n = 5;

    pipeArray = (tube *) malloc(sizeof(tube)*n);
    last = pipeArray + (n);
    int count = 0;
    for(aux = pipeArray; aux<last; aux++){
        pipe(aux->tuberia);
        printf("initialized %d pipe  \n", count);
        count++;
    }
    char f = 'T';

    for(int i = 1; i<=n; i++){
        pid = fork();

    if (pid == -1)
    {
        printf("Error al crear el proceso hijo");
        exit(-1);
    }
    else if (pid == 0)
    {
        printf("** Creación de hijo %d de pid %d \n", i, pid);
        char t;
        while(1){
            close((pipeArray+i-1)->tuberia[1]);
            read((pipeArray+i-1)->tuberia[0], &t, sizeof(char));
            printf("—-> Soy el proceso número %d con PID %d  y recibí el testigo %c, el cual tendré por 5 segundos \n", i, getpid(), t);
            sleep(5); 
            printf("<—- Soy el proceso número %d con PID %d y acabo de enviar el testigo %c \n", i, getpid(), t);
            if(i==n-1){
            close((pipeArray)->tuberia[0]);
            write((pipeArray)->tuberia[1], &t, sizeof(char));
            }else{
            close((pipeArray+i)->tuberia[0]);
            write((pipeArray+i)->tuberia[1], &t, sizeof(char));
            }
        }
        exit(0);
    }
    else {
        
        while(1){
            close((pipeArray+n)->tuberia[1]);
            read((pipeArray+n)->tuberia[0], &f, sizeof(char));
            printf("—-> Soy el proceso PADRE con PID %d  y recibí el testigo %c, el cual tendré por 5 segundos \n", getpid(), f);
            sleep(5); 
            printf("<—- Soy el proceso PADRE con PID %d y acabo de enviar el testigo %c \n", getpid(), f);
            close((pipeArray+1)->tuberia[0]);
            write((pipeArray+1)->tuberia[1], &f, sizeof(char));
        }
    }
    }
    
    return 0;
}