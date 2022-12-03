#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../includes/aplicacao.h"
#include "../includes/queue.h"
#include "../includes/socketC.h"

void *sendMessage(char quantidade_caracter[100])
{

    iniciaFila();
    while (1)
    {
        readMessage(quantidade_caracter);
    }
}

int main(int argc, char *argv[])
{
    // Thread para leitura da fila e escrita no socket - TO DO
    pthread_t socket;
    pthread_create(&socket, NULL, createSocket, NULL);

    // Thread para capturar mensagem do usuario

    if (argv[1] == 0)
    {
        printf("Digite bin/run (quantidade de caracter), ex: bin/run 50\n\n");
        exit(1);
    }
    pthread_t getMessage;
    pthread_create(&getMessage, NULL, sendMessage(argv[1]), NULL);

    pthread_join(getMessage, NULL); // Blocante
    pthread_join(socket, NULL);
    return 0;
}