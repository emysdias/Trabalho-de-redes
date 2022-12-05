# Trabalho-redes

## Simplex
Protocolo em modo simplex, ou seja, só um lado lê e o outro só recebe em canal livre de erros entre A e B.

## Alunos
Emily Dias Sousa - 18/0015966 </br>
Fellipe dos Santos Araujo - 18/0063723 </br>
Vitor Magalhães Lamego - 19/0020903

## Rodar projeto
É preciso abrir 2 terminais. </br>

No primeiro terminal rode:

```
cd listenMessage
```

Para rodar o make file rode:

```
make
```

```
bin/run 127.0.0.1 5000
```

No segundo terminal rode:

```
cd sendMessage
```

Para rodar o make file rode:

```
make
```

```
bin/run
```

## Adicionais
Para executar o projeto, é preciso ter um arquivo dentro da pasta sendMessage/files </br>

- Após o bin/run será feito umas perguntas como "Digite o nome do arquivo de mensagens: ", onde deve ser digitado o nome do arquivo, o arquivo teste é o 'mensagens.txt', para escolher ele, deve ser digitado somente o 'mensagens'.

- Depois do arquivo vem a pergunta "Digite o tamanho desejado para o quadro: ", onde aqui deve ser digitado o tamanho da área de dados da PDU de enlace desejado, exemplo 50, com isto, vai ser mostrado 50 caracteres no client. A negociação só termina quando os dois hosts chegam ao mesmo número de caracteres.

- A primeira comunicação possui um erro forçado em que um pacote não é enviado, forçando que a comunicação seja refeita. Por conta disso, aparece uma mensagem de que a comunicação falhou e será refeita.