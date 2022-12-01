#include <stdio.h>
#include <pthread.h>
#include "../includes/socketS.h"
#include <string.h>

int main(int argc, char *argv[])
{
    // Criar Socket de escuta
    pthread_t listenMessage;
    int res;
    res = pthread_create(&listenMessage, NULL, createSocket(argv[0], argv[1], argv[2]), NULL);

    pthread_join(listenMessage, NULL); // Blocante
}