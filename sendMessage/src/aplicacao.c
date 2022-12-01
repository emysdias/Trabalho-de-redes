#include <stdio.h>
#include "../includes/queue.h"

void *readMessage()
{
    char mensagem[200];
    printf("Mensagem: ");
    scanf(" %[^\n]", mensagem);
    insereFila(mensagem);
}