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

int negociouTamanho = 0;
int sd;

socklen_t tam_Cli;
struct sockaddr_in endCli;  /* Vai conter identificacao do cliente */
struct sockaddr_in endServ; /* Vai conter identificacao do servidor local */

void *createSocket(char ip_atual[MAX_MSG], char ip_server[MAX_MSG], char porta[10])
{
  int rc, n = 0;

  char msg[MAX_MSG]; /* Buffer que armazena os dados que chegaram via rede */

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
          deserializeMessage(&msg);
        }
      }
    }
  } /* fim do while */

} /* fim do programa */
