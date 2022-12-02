#include <stdio.h>
#include "../includes/queue.h"

#define SIZE 200

void *readMessage(char quantidade_caracter[100])
{
    char mensagem[SIZE];
    printf("Mensagem: ");
    scanf(" %[^\n]", mensagem);
    char palavra[SIZE];
    int j = -1;
    for (int i = 0; i < strlen(mensagem); i++)
    {
        j++;
        palavra[j] += mensagem[i];
        if (j == atoi(quantidade_caracter) - 1)
        {
            insereFila(palavra);
            memset(palavra, 0, SIZE);
            j = -1;
        }
    }
    if (j != -1)
    {
        insereFila(palavra);
    }
    memset(palavra, 0, SIZE);
}