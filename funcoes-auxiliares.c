#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "plota-grafico.h"

// Verifica se entrada é um diretório ou não;
int isDir(const char *fileName)
{
    struct stat path;
    stat(fileName, &path);
    return S_ISREG(path.st_mode);
}

// Faz a diferenca entre 2 timestamps e retorna o tempo em segundos;
int diferencaEntreDoisTimestamps(char *timestampAnterior, char *timestampAtual)
{
    int h, m, s;
    int tempoAnterior, tempoAtual;
    sscanf(timestampAnterior, "%d:%d:%d", &h, &m, &s);
    tempoAnterior = h * 3600 + m * 60 + s;
    sscanf(timestampAtual, "%d:%d:%d", &h, &m, &s);
    tempoAtual = h * 3600 + m * 60 + s;
    return tempoAtual - tempoAnterior;
}

// Percorre um diretório para saber quantidade de itens;
int verificaQntArquivosEmDiretorio(DIR *dirStream, char *caminhoDiretorio)
{
    int qntFiles = 0;
    struct dirent *dirEntry;
    char *caminhoMaisNomeArquivo, *nomeDoArquivo;

    if (!(caminhoMaisNomeArquivo = malloc(sizeof(char) * 1000))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    if (!(nomeDoArquivo = malloc(sizeof(char) * 200))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    // percorre o diretorio e acumula quantidade de arquivos;
    for (;;){
        if (!(dirEntry = readdir(dirStream))) break;
        strcpy(caminhoMaisNomeArquivo, caminhoDiretorio);
        strcpy(nomeDoArquivo, dirEntry->d_name);
        strcat(caminhoMaisNomeArquivo, nomeDoArquivo);
        // Se for arquivo, incrementa;
        if (isDir(caminhoMaisNomeArquivo) != 0){
            qntFiles++;
        }
    }
    // Reseta a posicao de uma stream de diretorio para o inicio do diretorio;
    rewinddir(dirStream);
    free(caminhoMaisNomeArquivo);
    free(nomeDoArquivo);
    return qntFiles;
}