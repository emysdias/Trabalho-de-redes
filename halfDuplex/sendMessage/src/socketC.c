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
#include "../includes/enlace.h"
#include "../includes/file.h"

#define IP_SERVIDOR "127.0.0.1"
#define PORTA "5000"
#define MAX_MSG 100
int sd, rc, i;

socklen_t tam_Cli;
struct sockaddr_in ladoCli;  /* dados do cliente local   */
struct sockaddr_in ladoServ; /* dados do servidor remoto */

int createSocketA()
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
  listening = 0;
  server = 0;
  communicateWithClient();
  return 1;
} /* fim do programa */

void *createSocketB()
{
  char ip_atual[50];
  strcpy(ip_atual, "SOCKET B");
  int rc, n = 0;

  /* Criacao do socket UDP */
  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0)
  {
    printf("%s: nao pode abrir o socket \n", ip_atual);
    exit(1);
  }

  /* Preenchendo informacoes sobre o servidor */
  ladoServ.sin_family = AF_INET;
  ladoServ.sin_addr.s_addr = inet_addr(IP_SERVIDOR);
  ladoServ.sin_port = htons(atoi(PORTA));

  /* Fazendo um bind na porta local do servidor */
  rc = bind(sd, (struct sockaddr *)&ladoServ, sizeof(ladoServ));
  if (rc < 0)
  {
    printf("%s: nao pode fazer bind na porta %s \n", ip_atual, PORTA);
    exit(1);
  }

  printf("%s: esperando por dados no IP: %s, porta UDP numero: %s\n", ip_atual, IP_SERVIDOR, PORTA);

  /* Este servidor entra num loop infinito esperando dados de clientes */
  listening = 1;
  server = 1;
  communicateWithClient();
}
