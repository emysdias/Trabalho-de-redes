/****************************************/
/* FGA/EngSofrware/FRC - udpServer.c    */
/* Prof. Fernando W. Cruz               */
/****************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>
#include "../includes/queue.h"
#include "../includes/file.h"

#define MAX_MSG 100
#define SIZE 300
#define IP_SERVIDOR "127.0.0.1"

int quantidadeCaracter = 0;
int negociouTamanho = 0;
int pacotesRecebidos = 0;

socklen_t tam_Cli;
struct sockaddr_in endCli;  /* Vai conter identificacao do cliente */
struct sockaddr_in endServ; /* Vai conter identificacao do servidor local */

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

void *createSocket(char ip_atual[MAX_MSG], char ip_server[MAX_MSG], char porta[10])
{
  int sd, rc, n = 0;
  char arquivo[300];
  char pacotes[10];

  char msg[MAX_MSG]; /* Buffer que armazena os dados que chegaram via rede */
  PDU pdu;

  /* Criacao do socket UDP */
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0)
  {
    printf("%s: nao pode abrir o socket \n", ip_atual);
    exit(1);
  }

  /* Preenchendo informacoes sobre o servidor */
  endServ.sin_family = AF_INET;
  endServ.sin_addr.s_addr = inet_addr(ip_server);
  endServ.sin_port = htons(atoi(porta));

  /* Fazendo um bind na porta local do servidor */
  rc = bind(sd, (struct sockaddr *)&endServ, sizeof(endServ));
  if (rc < 0)
  {
    printf("%s: nao pode fazer bind na porta %s \n", ip_atual, porta);
    exit(1);
  }

  printf("%s: esperando por dados no IP: %s, porta UDP numero: %s\n", ip_atual, ip_server, porta);

  /* Este servidor entra num loop infinito esperando dados de clientes */
  while (1)
  {
    /* inicia o buffer */
    memset(msg, 0x0, MAX_MSG);
    memset(arquivo, 0X0, 300);
    tam_Cli = sizeof(endCli);
    /* recebe a mensagem  */
    n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&endCli, &tam_Cli);
    if (n < 0)
    {
      printf("%s: nao pode receber dados \n", ip_atual);
      continue;
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
          negociaTamanho(msg, sd);
          apagarConteudoArquivo();
        }
        else
        {

          pdu = deserialize(&msg);

          if (!strcmp(pdu.data, "FIM"))
          {
            int finalId = atoi(pdu.id);
            sprintf(pacotes, "%d", pacotesRecebidos);
            printf("Enviando pacotes recebidos: %s\n", pacotes);
            rc = sendto(sd, pacotes, MAX_MSG, 0, (struct sockaddr *)&endCli, sizeof(endCli));
            if (finalId - 1 == pacotesRecebidos)
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
          else
          {
            strcat(arquivo, pdu.data);
            insereFila(pdu.data);
            pacotesRecebidos++;
            criaArquivo(arquivo);
          }
        }
      }
    }
  } /* fim do while */

} /* fim do programa */
