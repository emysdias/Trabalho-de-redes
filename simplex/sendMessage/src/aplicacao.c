#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../includes/queue.h"
#include "../includes/socketC.h"
#include "../includes/enlace.h"

#define SIZE 300

char quantidade_caracter[10] = "-1";
int envioOk = 0;
int pacotesEnviados = 0;

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
            pacotesEnviados++;
            memset(palavra, 0x0, SIZE);
            j = -1;
        }
        j++;
    }
    if (j != 0)
    {
        palavra[++j] = '\0';
        insereFila(palavra);
        pacotesEnviados++;
        memset(palavra, 0x0, SIZE);
    }
    j = 0;
}

FILE *leArquivo(char resposta[SIZE])
{
    char localArquivo[SIZE] = {"../sendMessage/files/"};
    return fopen(strcat(localArquivo, resposta), "r");
}

void *readMessage()
{
    int j = 0;
    char mensagem[SIZE];

    char resposta[SIZE];
    char palavra[SIZE];
    char *conteudoArquivo;
    FILE *file;
    int count = 0;

    printf("Digite o nome do arquivo de mensagens: "); // exemplo 'mensagens'
    scanf(" %[^\n]", resposta);

    strcat(resposta, ".txt");

    file = leArquivo(resposta);
    if (file == NULL) // le arquivo digitado
    {
        printf("Arquivo não existe\n");
        exit(1);
    }

    mandaNomeArquivo(resposta);
    negociaTamanhoQuadro();

    if (strcmp("-1", quantidade_caracter))
    {
        conteudoArquivo = (int *)malloc(atoi(quantidade_caracter) * SIZE);
        printf("Tamanho utilizado: %s\n", quantidade_caracter);
        int tamanhoLimite = atoi(quantidade_caracter);
        while (!envioOk)
        {
            pacotesEnviados = 0;
            file = leArquivo(resposta);
            while (fscanf(file, " %[^\n]", conteudoArquivo) != EOF)
            {
                strcat(conteudoArquivo, "\n");
                cutMessage(conteudoArquivo, tamanhoLimite);
            }
            insereFila("FIM");
            envioOk = checaEnvioArquivo(pacotesEnviados);
            id_geral = 0;
        }
        negociouTamanho = 0;
        envioOk = 0;
    }
    fclose(file);
}

void *readFile()
{
    iniciaFila();
    while (1)
    {
        readMessage();
    }
}