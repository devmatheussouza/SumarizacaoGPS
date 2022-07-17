#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include "funcoes-auxiliares.h"

int main(int argc, char **argv)
{
    DIR *dirStream;
    char *caminho = argv[2];
    logs *vetorLogsGeral;
    bikes *vetBikes;
    int qntBikes, qntLogs; 

    if (caminho[strlen(caminho) - 1] != '/')
        strcat(caminho, "/");

    if (!(dirStream = opendir(caminho))){
        perror("Nao foi possivel abrir o diretorio.");
        exit(1);
    }

    qntLogs = verificaQntArquivosEmDiretorio(dirStream, caminho);
    
    if (!(vetorLogsGeral = malloc(sizeof(logs) * qntLogs))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    inicializaVetorLogs(vetorLogsGeral, qntLogs);

    manipulaArquivosLog(dirStream, caminho, vetorLogsGeral, qntLogs);

    ordenaVetorLogsPorNome(vetorLogsGeral, qntLogs);
    
    vetBikes = preencheVetorBikes(vetorLogsGeral, qntLogs, &qntBikes);

    interacaoResultadosComUsuario(vetorLogsGeral, vetBikes, qntBikes, qntLogs);

    freeVetorLogs(vetorLogsGeral);

    freeVetorBikes(vetBikes);

    (void)closedir(dirStream);
    return 0;
}