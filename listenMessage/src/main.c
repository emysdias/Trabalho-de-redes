#include <stdio.h>
#include <pthread.h>
#include "../includes/socketS.h"
#include <string.h>
#include <unistd.h>
#include "../includes/queue.h"

void *receiveMessage() {
    char message[300];

    while(1) {
        // sleep(2);
        if (vaziaFila() == 0) {
            char *buffer = retiraFila();
            memcpy(message, buffer, strlen(buffer));
            printf("Mensagem recebida => %s\n", message);
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