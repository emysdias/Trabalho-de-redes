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

#define MAX_MSG 300
#define IP_SERVIDOR "127.0.0.1"
#define PORTA "5000"
int sd, rc, i;

struct sockaddr_in ladoCli;  /* dados do cliente local   */
struct sockaddr_in ladoServ; /* dados do servidor remoto */

void sendMessageSocket(char mensagem[300])
{
  int rc;
  rc = sendto(sd, mensagem, strlen(mensagem), 0, (struct sockaddr *)&ladoServ, sizeof(ladoServ));
  if (rc < 0)
  {
    printf("%s: nao pode enviar dados %d \n", IP_SERVIDOR, i - 1);
    close(sd);
    exit(1);
  }
  // printf("Enviando parametro: %s\n", mensagem);
} /* fim do for (laco) */

void *consumeQueue()
{
  while (1)
  {
    if (vaziaFila() == 0)
      sendMessageSocket(retiraFila());
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
