#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../includes/aplicacao.h"
#include "../includes/queue.h"

void *sendMessage()
{

    while (1)
    {
        inicia();
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

    pthread_join(getMessage, NULL); // Blocante

    // Thread para leitura da fila e escrita no socket
    return 0;
}