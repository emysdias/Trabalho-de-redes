#include <stdio.h>
#include <pthread.h>
#include "../includes/socketS.h"
#include <string.h>
#include <unistd.h>
#include "../includes/queue.h"
#include "../includes/socketS.h"

void *receiveMessage()
{
    char message[300];
    iniciaFila();

    while (1)
    {
        if (!vaziaFila())
        {
            printf("Mensagem recebida => %s\n", retiraFila());
                }
    }
}

int main(int argc, char *argv[])
{
    // Criar Socket de escuta
    pthread_t listenMessage, receiveAndReadMessage;

    pthread_create(&receiveAndReadMessage, NULL, receiveMessage, NULL);
    pthread_create(&listenMessage, NULL, createSocket(argv[0], argv[1], argv[2]), NULL);

    pthread_join(receiveAndReadMessage, NULL);
    pthread_join(listenMessage, NULL); // Blocante

    return 0;
}