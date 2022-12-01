#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/queue.h"

struct Node
{
    char msg[300];
    struct Node *prox;
};
int tam = 0;

node *FILA;

void inicia();
int vazia();
node *aloca();
void insere(char[300]);
char *retira();
// void exibe(node *FILA);
void libera();

void inicia()
{
    FILA = (node *)malloc(sizeof(node));
    FILA->prox = NULL;
}

int vazia()
{
    if (FILA->prox == NULL)
        return 1;
    else
        return 0;
}

node *aloca(char msg[300])
{
    node *novo = (node *)malloc(sizeof(node));
    if (!novo)
    {
        printf("Sem memoria disponivel!\n");
        exit(1);
    }
    else
    {
        strcpy(novo->msg, msg);
        return novo;
    }
}

void insere(char msg[300])
{
    node *novo = aloca(msg);
    novo->prox = NULL;

    if (vazia(FILA))
        FILA->prox = novo;
    else
    {
        node *tmp = FILA->prox;

        while (tmp->prox != NULL)
            tmp = tmp->prox;

        tmp->prox = novo;
    }
    tam++;
    printf("Tamanho da fila: %d\n", tam);
}

char *retira()
{
    if (FILA->prox == NULL)
    {
        printf("Fila ja esta vazia\n");
        return NULL;
    }
    else
    {
        node *tmp = FILA->prox;
        FILA->prox = tmp->prox;
        tam--;
        return tmp->msg;
    }
}

// void exibe(node *FILA)
// {
//     if (vazia(FILA))
//     {
//         printf("Fila vazia!\n\n");
//         return;
//     }

//     node *tmp;
//     tmp = FILA->prox;
//     printf("Fila :");
//     while (tmp != NULL)
//     {
//         printf("%5d", tmp->num);
//         tmp = tmp->prox;
//     }
//     printf("\n        ");
//     int count;
//     for (count = 0; count < tam; count++)
//         printf("  ^  ");
//     printf("\nOrdem:");
//     for (count = 0; count < tam; count++)
//         printf("%5d", count + 1);

//     printf("\n\n");
// }

void libera()
{
    if (!vazia(FILA))
    {
        node *proxNode,
            *atual;

        atual = FILA->prox;
        while (atual != NULL)
        {
            proxNode = atual->prox;
            free(atual);
            atual = proxNode;
        }
    }
}