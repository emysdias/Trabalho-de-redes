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
    int isHostA = atoi(argv[1]);
    if (isHostA)
        pthread_create(&socket, NULL, createSocketA, NULL);
    else
        pthread_create(&socket, NULL, createSocketB, NULL);

    pthread_join(socket, NULL);
    return 0;
}