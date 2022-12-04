/***********************************/
/* FGA/EngSofware/FRC  udpClient.c */
/* Prof. Fernando W Cruz           */
/* Código modificado               */
/***********************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <stdlib.h>
#include "../includes/queue.h"
#include "../includes/aplicacao.h"

#define MAX_MSG 300
#define IP_SERVIDOR "127.0.0.1"
#define PORTA "5000"
int sd, rc, i;

struct sockaddr_in ladoCli;  /* dados do cliente local   */
struct sockaddr_in ladoServ; /* dados do servidor remoto */
int negociouTamanho = 0;

typedef struct
{
  char *source;
  char *target;
  char *data;
  char *lenght;
} PDU;

char *serialize(PDU pdu, int *buffer_len)
{
  int source_len = strlen(pdu.source);
  int target_len = strlen(pdu.target);
  int data_len = strlen(pdu.data);
  int lenght_len = strlen(pdu.lenght);

  int size = 4 * sizeof(int) + (source_len + target_len + data_len + lenght_len);

  char *buf = malloc(sizeof(char) * (size + 1));

  memcpy(buf, &source_len, sizeof(int));
  memcpy(buf + sizeof(int), pdu.source, source_len);
  memcpy(buf + sizeof(int) + source_len, &target_len, sizeof(int));
  memcpy(buf + 2 * sizeof(int) + source_len, pdu.target, target_len);
  memcpy(buf + 2 * sizeof(int) + source_len + target_len, &data_len, sizeof(int));
  memcpy(buf + 3 * sizeof(int) + source_len + target_len, pdu.data, data_len);
  memcpy(buf + 3 * sizeof(int) + source_len + target_len + data_len, &lenght_len, sizeof(int));
  memcpy(buf + 4 * sizeof(int) + source_len + target_len + data_len, pdu.lenght, lenght_len);
  buf[size] = '\0';

  *buffer_len = size;

  return buf;
}

void sendMessageSocket(char mensagem[MAX_MSG])
{
  PDU pdu;
  pdu.source = "127.0.0.1";
  pdu.target = "127.0.0.1";
  pdu.data = mensagem;
  pdu.lenght = "50";

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
  // printf("Enviando parametro: %s\n", mensagem);
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
  while (1)
  {
    if (negociouTamanho)
    {
      if (vaziaFila() == 0)
        sendMessageSocket(retiraFila());
    }
  }
}

int createSocket()
{
  /* Preenchendo as informacoes de identificacao do remoto */
  ladoServ.sin_family = AF_INET;
  ladoServ.sin_addr.s_addr = inet_addr(IP_SERVIDOR);
  ladoServ.sin_port = htons(atoi(PORTA));

  /* Preenchendo as informacoes de identificacao do cliente */
  ladoCli.sin_family = AF_INET;
  ladoCli.sin_addr.s_addr = htonl(INADDR_ANY);
  ladoCli.sin_port = htons(0); /* usa porta livre entre (1024-5000)*/

  /* Criando um socket. Nesse momento a variavel       */
  /* sd contem apenas dados sobre familia e protocolo  */
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0)
  {
    printf("%s: nao pode abrir o socket \n", IP_SERVIDOR);
    exit(1);
  }

  /* Relacionando o socket sd com a estrutura ladoCli */
  /* Depois do bind, sd faz referencia a protocolo local, ip local e porta local */
  rc = bind(sd, (struct sockaddr *)&ladoCli, sizeof(ladoCli));
  if (rc < 0)
  {
    printf("%s: nao pode fazer um bind da porta\n", IP_SERVIDOR);
    exit(1);
  }
  // printf("{UDP, IP_Cli: %s, Porta_Cli: %u, IP_R: %s, Porta_R: %s}\n", inet_ntoa(ladoCli.sin_addr), ntohs(ladoCli.sin_port), IP_SERVIDOR, PORTA);
  consumeQueue();
  return 1;
} /* fim do programa */
