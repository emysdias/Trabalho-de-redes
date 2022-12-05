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
#include "../includes/file.h"

int negociouTamanho = 0;
int id_geral = 0;

int quantidadeCaracter = 0;
int pacotesRecebidos = 0;
int forca_erro = 1;
int listening;
int server;

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
    char aux[10];
    PDU pdu;
    pdu.source = "127.0.0.1";
    pdu.target = "127.0.0.1";
    pdu.data = mensagem;
    pdu.lenght = "50";
    sprintf(aux, "%d", ++id_geral);
    pdu.id = aux;

    int buffer_len = 0;
    char *buffer = serialize(pdu, &buffer_len);

    int rc;
    if (server)
        rc = sendto(sd, buffer, MAX_MSG, 0, (struct sockaddr *)&ladoCli, sizeof(ladoCli));
    else
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

    if (server)
        rc = sendto(sd, resposta, MAX_MSG, 0, (struct sockaddr *)&ladoCli, sizeof(ladoCli));
    else
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

    strcpy(quantidade_caracter, "-1");
    while (!strcmp(quantidade_caracter, "-1"))
    {
        printf("Digite o tamanho desejado para o quadro: ");
        scanf(" %s", tamanho);

        int rc;
        if (server)
            rc = sendto(sd, tamanho, MAX_MSG, 0, (struct sockaddr *)&ladoCli, sizeof(ladoCli));
        else
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
}

void negociaTamanhoListening(char msg[10], int sd)
{
    int rc, i;
    char tamanho[10];
    printf("Cliente deseja mandar mensagens de tamanho: %s\n", msg);
    printf("Qual o tamanho deseja receber as mensagens: ");
    scanf(" %s", tamanho);
    quantidadeCaracter = atoi(tamanho);

    if (server)
        rc = sendto(sd, tamanho, MAX_MSG, 0, (struct sockaddr *)&ladoCli, sizeof(ladoCli));
    else
        rc = sendto(sd, tamanho, MAX_MSG, 0, (struct sockaddr *)&ladoServ, sizeof(ladoServ));
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
    }
}

void *printMessagesReceived()
{
    while (!vaziaFila())
    {
        printf("Mensagem recebida => %s\n", retiraFila());
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
        int finalId = atoi(pdu.id);
        sprintf(pacotes, "%d", pacotesRecebidos);
        printf("Enviando pacotes recebidos: %s\n", pacotes);
        printMessagesReceived();

        if (server)
            rc = sendto(sd, pacotes, MAX_MSG, 0, (struct sockaddr *)&ladoCli, sizeof(ladoCli));
        else
            rc = sendto(sd, pacotes, MAX_MSG, 0, (struct sockaddr *)&ladoServ, sizeof(ladoServ));
        if (finalId - 1 == pacotesRecebidos)
        {
            negociouTamanho = 0;
            recebeNomeArquivo = 0;
            pacotesRecebidos = 0;
            listening = 0;
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

void *listenOtherSide()
{
    char msg[MAX_MSG]; /* Buffer que armazena os dados que chegaram via rede */

    /* inicia o buffer */
    memset(msg, 0x0, MAX_MSG);
    socklen_t tam_Cli = sizeof(ladoCli);
    socklen_t tam_Ser = sizeof(ladoServ);
    /* recebe a mensagem  */
    while (listening)
    {
        int n;
        if (server)
            n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&ladoCli, &tam_Cli);
        else
            n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&ladoServ, &tam_Ser);
        if (n < 0)
        {
            printf("%s: nao pode receber dados \n", "SOCKET");
        }
        else
        {
            if (!recebeNomeArquivo)
            {
                pegarNomeArquivo(msg);
            }
            else
            {
                if (!negociouTamanho)
                {
                    negociaTamanhoListening(msg, sd);
                    apagarConteudoArquivo();
                }
                else
                {
                    deserializeMessage(&msg);
                }
            }
        }
    }
}

void *consumeQueue()
{
    iniciaFila();
    while (!vaziaFila())
    {
        sendMessageSocket(retiraFila());
    }
}
