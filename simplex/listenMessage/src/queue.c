#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/queue.h"
#include "../includes/socketS.h"

int tam = 0;
#define SIZE 300

node *FILA;

void iniciaFila();
int vaziaFila();
node *alocaFila();
void insereFila(char[SIZE]);
char *retiraFila();
// void exibe(node *FILA);
void liberaFila();

void iniciaFila()
{
    FILA = (node *)malloc(sizeof(node));
    FILA->prox = NULL;
}

int vaziaFila()
{
    if (FILA->prox == NULL)
        return 1;
    else
        return 0;
}

node *alocaFila(char msg[SIZE])
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

void insereFila(char msg[SIZE])
{
    node *novo = alocaFila(msg);
    novo->prox = NULL;

    if (vaziaFila())
        FILA->prox = novo;
    else
    {
        node *tmp = FILA->prox;

        while (tmp->prox != NULL)
            tmp = tmp->prox;

        tmp->prox = novo;
    }
    tam++;
}

char *retiraFila()
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

void liberaFila()
{
    if (!vaziaFila(FILA))
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