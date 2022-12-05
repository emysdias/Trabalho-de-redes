#include <stdio.h>
#include <stdlib.h>

int recebeNomeArquivo = 0;
char nomeArquivo[100];
FILE *file;

void pegarNomeArquivo(char msg[10])
{
    strcpy(nomeArquivo, msg);
    recebeNomeArquivo = 1;
}

void apagarConteudoArquivo()
{
    char localArquivo[300] = {"../sendMessage/resultFile/"};
    if ((file = fopen(strcat(localArquivo, nomeArquivo), "w")) == NULL) // le arquivo digitado
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    fclose(file);
}

void criaArquivo(char msg[300])
{
    char localArquivo[300] = {"../sendMessage/resultFile/"};
    if ((file = fopen(strcat(localArquivo, nomeArquivo), "a+")) != NULL) // le arquivo digitado
    {
        fprintf(file, "%s", msg);
    }
    else
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    fclose(file);
}