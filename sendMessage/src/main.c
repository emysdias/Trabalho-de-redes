#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../includes/aplicacao.h"
#include "../includes/queue.h"
#include "../includes/socketC.h"

void *sendMessage()
{
    iniciaFila();
    while (1)
    {
        readMessage();
    }
}

int main()
{
    // Thread para leitura da fila e escrita no socket - TO DO
    pthread_t socket;
    int res1;
    res1 = pthread_create(&socket, NULL, createSocket, NULL);

    // Thread para capturar mensagem do usuario
    pthread_t getMessage;
    int res;
    res = pthread_create(&getMessage, NULL, sendMessage, NULL);

    pthread_join(getMessage, NULL); // Blocante
    pthread_join(socket, NULL);
    return 0;
}