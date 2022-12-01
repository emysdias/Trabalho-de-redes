#include <stdio.h>
#include "../includes/queue.h"

void *readMessage()
{
    char mensagem[200];
    printf("Mensagem:\n");
    scanf(" %[^\n]", mensagem);
    insereFila(mensagem);
}