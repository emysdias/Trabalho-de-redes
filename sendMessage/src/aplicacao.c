#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "../includes/queue.h"
#include "../includes/socketC.h"

#define SIZE 300

char quantidade_caracter[10] = "-1";

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
        while (fscanf(file, " %[^\n]", conteudoArquivo) != EOF)
        {
            insereFila(conteudoArquivo);
        }
    }
    fclose(file);
}