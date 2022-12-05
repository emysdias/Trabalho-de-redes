#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/queue.h"
#include "../includes/file.h"
#include "../includes/socketS.h"

#define MAX_MSG 100
#define SIZE 300
#define IP_SERVIDOR "127.0.0.1"

int quantidadeCaracter = 0;
int pacotesRecebidos = 0;
int forca_erro = 1;

typedef struct
{
    char *source;
    char *target;
    char *data;
    char *lenght;
    char *id;
} PDU;

PDU deserialize(char *buf)
{
    PDU pdu;
    int source_len;
    int target_len;
    int data_len;
    int lenght_len;
    int id_len;

    memcpy(&source_len, buf, sizeof(int));
    memcpy(&target_len, buf + sizeof(int) + source_len, sizeof(int));
    memcpy(&data_len, buf + 2 * sizeof(int) + source_len + target_len, sizeof(int));
    memcpy(&lenght_len, buf + 3 * sizeof(int) + source_len + target_len + data_len, sizeof(int));
    memcpy(&id_len, buf + 4 * sizeof(int) + source_len + target_len + data_len + lenght_len, sizeof(int));

    pdu.source = malloc(source_len * sizeof(char));
    pdu.target = malloc(target_len * sizeof(char));
    pdu.data = malloc(data_len * sizeof(char));
    pdu.lenght = malloc(lenght_len * sizeof(char));
    pdu.id = malloc(id_len * sizeof(char));

    memcpy(pdu.source, buf + sizeof(int), source_len);
    memcpy(pdu.target, buf + 2 * sizeof(int) + source_len, target_len);
    memcpy(pdu.data, buf + 3 * sizeof(int) + source_len + target_len, data_len);
    memcpy(pdu.lenght, buf + 4 * sizeof(int) + source_len + target_len + data_len, lenght_len);
    memcpy(pdu.id, buf + 5 * sizeof(int) + source_len + target_len + data_len + lenght_len, id_len);

    return pdu;
}

void negociaTamanho(char msg[10], int sd)
{
    int rc, i;
    char tamanho[10];
    printf("Cliente deseja mandar mensagens de tamanho: %s\n", msg);
    printf("Qual o tamanho deseja receber as mensagens: ");
    scanf(" %s", tamanho);
    quantidadeCaracter = atoi(tamanho);

    rc = sendto(sd, tamanho, MAX_MSG, 0, (struct sockaddr *)&endCli, sizeof(endCli));
    if (rc < 0)
    {
        printf("%s: nao pode enviar dados %d \n", IP_SERVIDOR, i - 1);
        close(sd);
        exit(1);
    }

    if (!strcmp(tamanho, msg))
    {
        printf("Tamanho negociado com sucesso!\n");
        negociouTamanho = 1;
    }
    else
    {
        printf("Tamanho nao negociado\n");
        recebeNomeArquivo = 0;
    }
}

void deserializeMessage(char *msg)
{
    PDU pdu;
    char arquivo[300];
    char pacotes[10];
    int rc;

    memset(arquivo, 0X0, 300);

    pdu = deserialize(msg);
    if (!strcmp(pdu.data, "FIM"))
    {
        int pacotesEnviados = atoi(pdu.id);
        sprintf(pacotes, "%d", pacotesRecebidos);
        printf("Enviando pacotes recebidos: %s\n", pacotes);
        rc = sendto(sd, pacotes, MAX_MSG, 0, (struct sockaddr *)&endCli, sizeof(endCli));
        printf("PacotesEnviados: %d PacotesRecebidos: %d\n", pacotesEnviados, pacotesRecebidos);
        if (pacotesEnviados == pacotesRecebidos)
        {
            negociouTamanho = 0;
            recebeNomeArquivo = 0;
            pacotesRecebidos = 0;
        }
        else
        {
            printf("Erro na comunicacao!! Aguardando envio das mensagens novamente!\n\n");
            pacotesRecebidos = 0;
        }
    }
    else if (forca_erro && pacotesRecebidos == 4)
    {
        forca_erro = 0;
    }
    else
    {
        strcat(arquivo, pdu.data);
        insereFila(pdu.data);
        pacotesRecebidos++;
        criaArquivo(arquivo);
    }
}
