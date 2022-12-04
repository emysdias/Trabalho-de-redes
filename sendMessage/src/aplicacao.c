#include <stdio.h>
#include <sys/stat.h>
#include "../includes/queue.h"
#include "../includes/socketC.h"

#define SIZE 300

char quantidade_caracter[10];

void *readMessage(char quantidade_caracter[100])
{
    int j = 0;
    char mensagem[SIZE];
    char localArquivo[SIZE] = {"../sendMessage/files/"};
    char resposta[SIZE];
    char palavra[SIZE];
    char *conteudoArquivo = (int *)malloc(sizeof(int));
    FILE *file;
    int count = 0;

    printf("Digite o nome do arquivo de mensagens: "); // exemplo 'mensagens'
    scanf(" %[^\n]", resposta);
    strcat(resposta, ".txt");

    if ((file = fopen(strcat(localArquivo, resposta), "r")) == NULL) // le arquivo digitado
    {
        printf("Arquivo não existe\n");
        exit(1);
    }

    negociaTamanhoQuadro();

    while (fscanf(file, " %[^\n]", conteudoArquivo) != EOF)
    {
        strcpy(mensagem, conteudoArquivo);
        memset(palavra, 0x0, SIZE);
        for (int i = 0; i < strlen(mensagem); i++)
        {
            palavra[j] = mensagem[i];

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
        j = 0;
    }
    fclose(file);
}