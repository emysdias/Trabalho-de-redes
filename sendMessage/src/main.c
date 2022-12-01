#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../includes/aplicacao.h"
#include "../includes/queue.h"
#include "../includes/socketC.h"

void *sendMessage()
{
    while (1)
    {
        iniciaFila();
        readMessage();
    }
}

int main()
{
    // Thread para capturar mensagem do usuario
    pthread_t getMessage;
    int res;
    printf("Teste\n");
    res = pthread_create(&getMessage, NULL, sendMessage, NULL);

    // Thread para leitura da fila e escrita no socket - TO DO
    pthread_t socket;
    int res1;
    res1 = pthread_create(&socket, NULL, createSocket, NULL);

    pthread_join(getMessage, NULL); // Blocante
    pthread_join(socket, NULL);
    return 0;
}