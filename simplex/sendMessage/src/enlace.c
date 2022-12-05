#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../includes/queue.h"
#include "../includes/aplicacao.h"
#include "../includes/socketC.h"

int negociouTamanho = 0;
int id_geral = 0;
#define MAX_MSG 300
#define IP_SERVIDOR "127.0.0.1"

typedef struct
{
    char *source;
    char *target;
    char *data;
    char *lenght;
    char *id;
} PDU;

char *serialize(PDU pdu, int *buffer_len)
{
    int source_len = strlen(pdu.source);
    int target_len = strlen(pdu.target);
    int data_len = strlen(pdu.data);
    int lenght_len = strlen(pdu.lenght);
    int id_len = strlen(pdu.id);

    int size = 5 * sizeof(int) + (source_len + target_len + data_len + lenght_len + id_len);

    char *buf = malloc(sizeof(char) * (size + 1));

    memcpy(buf, &source_len, sizeof(int));
    memcpy(buf + sizeof(int), pdu.source, source_len);

    memcpy(buf + sizeof(int) + source_len, &target_len, sizeof(int));
    memcpy(buf + 2 * sizeof(int) + source_len, pdu.target, target_len);

    memcpy(buf + 2 * sizeof(int) + source_len + target_len, &data_len, sizeof(int));
    memcpy(buf + 3 * sizeof(int) + source_len + target_len, pdu.data, data_len);

    memcpy(buf + 3 * sizeof(int) + source_len + target_len + data_len, &lenght_len, sizeof(int));
    memcpy(buf + 4 * sizeof(int) + source_len + target_len + data_len, pdu.lenght, lenght_len);

    memcpy(buf + 4 * sizeof(int) + source_len + target_len + data_len + lenght_len, &id_len, sizeof(int));
    memcpy(buf + 5 * sizeof(int) + source_len + target_len + data_len + lenght_len, pdu.id, id_len);
    buf[size] = '\0';

    *buffer_len = size;

    return buf;
}

int checaEnvioArquivo(int pacotesEnviados)
{
    char pacotesRecebidos[10];
    memset(pacotesRecebidos, 0x0, 10);
    recv(sd, pacotesRecebidos, sizeof(pacotesRecebidos), 0);

    int pacotes = atoi(pacotesRecebidos);
    printf("Pacotes recebidos: %d Pacotes Enviados: %d\n", pacotes, pacotesEnviados);
    if (pacotes != pacotesEnviados)
    {
        printf("Problema com a comunicacao! Sera realizada uma nova tentativa\n");
        return 0;
    }
    return 1;
}

void sendMessageSocket(char mensagem[MAX_MSG])
{
    char aux[10], len[10];
    int tam = strlen(mensagem);
    PDU pdu;
    pdu.source = "127.0.0.1";
    pdu.target = IP_SERVIDOR;
    pdu.data = mensagem;
    sprintf(len, "%d", tam);
    pdu.lenght = len;
    sprintf(aux, "%d", ++id_geral);
    pdu.id = aux;

    if (!strcmp(mensagem, "FIM"))
    {
        sprintf(aux, "%d", pacotesEnviados);
        pdu.id = aux;
    }
    int buffer_len = 0;
    char *buffer = serialize(pdu, &buffer_len);

    int rc;
    rc = sendto(sd, buffer, MAX_MSG, 0, (struct sockaddr *)&ladoServ, sizeof(ladoServ));
    if (rc < 0)
    {
        printf("%s: nao pode enviar dados %d \n", IP_SERVIDOR, i - 1);
        close(sd);
        exit(1);
    }
    if (strcmp(pdu.data, "FIM"))
        negociouTamanho = 0;
} /* fim do for (laco) */

void mandaNomeArquivo(char resposta[MAX_MSG])
{
    int rc;

    rc = sendto(sd, resposta, MAX_MSG, 0, (struct sockaddr *)&ladoServ, sizeof(ladoServ));
    if (rc < 0)
    {
        printf("%s: nao pode enviar dados %d \n", IP_SERVIDOR, i - 1);
        close(sd);
        exit(1);
    }
}

void negociaTamanhoQuadro()
{
    char tamanho[10];
    char tamanhoRecebido[MAX_MSG];

    printf("Digite o tamanho desejado para o quadro: ");
    scanf(" %s", tamanho);

    int rc;
    rc = sendto(sd, tamanho, MAX_MSG, 0, (struct sockaddr *)&ladoServ, sizeof(ladoServ));

    if (rc < 0)
    {
        printf("%s: nao pode enviar dados %d \n", IP_SERVIDOR, i - 1);
        close(sd);
        exit(1);
    }
    /* recebe a mensagem  */
    printf("Esperando confirmacao do server\n");
    int n;

    memset(tamanhoRecebido, 0x0, MAX_MSG);
    recv(sd, tamanhoRecebido, sizeof(tamanhoRecebido), 0);

    if (!strcmp(tamanho, tamanhoRecebido))
    {
        printf("Tamanho aceito!\n");
        negociouTamanho = 1;
        strcpy(quantidade_caracter, tamanho);
    }
    else
    {
        printf("Tamanho recusado! Tamanho solicitado: %s\n", tamanhoRecebido);
        strcpy(quantidade_caracter, "-1");
    }
}

void *consumeQueue()
{
    iniciaFila();
    while (1)
    {
        if (!vaziaFila())
            sendMessageSocket(retiraFila());
    }
}