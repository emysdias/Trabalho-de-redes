typedef struct Node node;
extern struct Node
{
    char msg[300];
    struct Node *prox;
};
extern node *FILA;
extern void insereFila(char[300]);
extern void iniciaFila();
extern int vaziaFila();
extern char *retiraFila();
