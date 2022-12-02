#include <stdio.h>
#include "../includes/queue.h"

#define SIZE 200

void *readMessage(char quantidade_caracter[100])
{
    char mensagem[SIZE];
    printf("Mensagem: ");
    scanf(" %[^\n]", mensagem);
    char palavra[SIZE];
    int j = 0;
    memset(palavra, 0x0, SIZE);
    for (int i = 0; i < strlen(mensagem); i++)
    {
        palavra[j] += mensagem[i];
        if (j == atoi(quantidade_caracter) - 1)
        {
            insereFila(palavra);
            memset(palavra, 0x0, SIZE);
            j = -1;
        }
        j++;
    }
    if (j != 0)
    {
        insereFila(palavra);
        memset(palavra, 0x0, SIZE);
    }
}