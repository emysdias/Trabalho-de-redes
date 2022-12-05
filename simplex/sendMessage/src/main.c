#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../includes/aplicacao.h"
#include "../includes/queue.h"
#include "../includes/socketC.h"

int main(int argc, char *argv[])
{
    // Thread para leitura da fila e escrita no socket
    pthread_t socket;
    pthread_create(&socket, NULL, createSocket, NULL);

    // Thread para capturar arquivo
    pthread_t getMessage;
    pthread_create(&getMessage, NULL, readFile, NULL);

    pthread_join(getMessage, NULL); // Blocante
    pthread_join(socket, NULL);
    return 0;
}