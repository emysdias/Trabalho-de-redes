#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../includes/queue.h"
#include "../includes/socketC.h"

#define SIZE 300

char quantidade_caracter[10] = "-1";

void cutMessage(char *mensagem, int quantidadeCaracter)
{
    char palavra[SIZE];
    int j = 0;

    for (int i = 0; i < strlen(mensagem); i++)
    {
        palavra[j] = mensagem[i];

        if (j == quantidadeCaracter - 2)
        {
            palavra[++j] = '\0';
            insereFila(palavra);
            memset(palavra, 0x0, SIZE);
            j = -1;
        }
        j++;
    }
    if (j != 0)
    {
        palavra[++j] = '\0';
        insereFila(palavra);
        memset(palavra, 0x0, SIZE);
    }
    j = 0;
}

void *readMessage()
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

    mandaNomeArquivo(resposta);
    negociaTamanhoQuadro();

    if (strcmp("-1", quantidade_caracter))
    {
        printf("Tamanho utilizado: %s\n", quantidade_caracter);
        int tamanhoLimite = atoi(quantidade_caracter);
        char mensagem[tamanhoLimite + 1];
        while (fscanf(file, " %[^\n]", conteudoArquivo) != EOF)
        {
            strcat(conteudoArquivo, "\n");
            cutMessage(conteudoArquivo, tamanhoLimite);
        }
    }
    fclose(file);
}