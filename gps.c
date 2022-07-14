#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "funcoes-auxiliares.h"
#include "manipula-logs.h"

int main(int argc, char **argv)
{
    DIR *dirStream;
    char *caminho = argv[2];
    logs *vetorLogsGeral;
    bikes *vetBikes;
    int qntBikes, i, qntLogs, opcaoUser, modeloEscolhido;

    if (caminho[strlen(caminho) - 1] != '/')
        strcat(caminho, "/");

    if (!(dirStream = opendir(caminho))){
        perror("Nao foi possivel abrir o diretorio.");
        exit(1);
    }

    printf("Realizando leitura dos arquivos de log...\n");

    qntLogs = verificaQntArquivosEmDiretorio(dirStream, caminho);
    
    if (!(vetorLogsGeral = malloc(sizeof(logs) * qntLogs))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    inicializaVetorLogs(vetorLogsGeral, qntLogs);

    manipulaArquivosLog(dirStream, caminho, vetorLogsGeral, &qntLogs);

    ordenaVetorLogs(vetorLogsGeral, qntLogs);

    printaSumario(vetorLogsGeral, qntLogs);

    vetBikes = preencheVetorBikes(vetorLogsGeral, qntLogs, &qntBikes);

    opcaoUser = 1;
    while (opcaoUser != 0)
    {
        printf("Digite um numero para realizar uma das seguintes funcoes:\n\n");
        printf("1) Mostrar modelos de bike encontrados\n");
        printf("2) Escolher um modelo de bike e ver atividades\n");
        printf("3) Listar todas atividades agrupadas por bicicleta e ordenadas pela data\n");
        printf("4) Listar todas atividades agrupadas por bicicleta e ordenadas pela distancia\n");
        printf("5) Listar todas atividades ordenadas pela subida acumulada\n");
        printf("6) Escolher um modelo de bike e mostrar um histograma da distribuicao das distancias\n");
        printf("0) Sair \n");
        printf("\n");
        printf("Acao desejada: ");
        scanf("%d", &opcaoUser);
        switch (opcaoUser)
        {
        case 0:
            printf("\n");
            printf("Até mais\n");
            break;

        case 1:
            printaNomesBikes(vetBikes, qntBikes);
            break;

        case 2:
            modeloEscolhido = funcaoModeloBikeSwitchCase(vetBikes, qntBikes);
            printAgrupadoPorBicicleta(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printf("\n");
            break;

        case 3:
            modeloEscolhido = funcaoModeloBikeSwitchCase(vetBikes, qntBikes);
            ordenaAtividadesBicicletaPorData(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printAgrupadoPorBicicleta(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printf("\n");
            break;

        case 4:
            modeloEscolhido = funcaoModeloBikeSwitchCase(vetBikes, qntBikes);
            ordenaAtividadesBicicletaPorDistancia(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printAgrupadoPorBicicleta(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printf("\n");
            break;

        case 5:
            ordenaAtividadesBicicletaPorSubidaAcumulada(vetorLogsGeral, 0, qntLogs);
            printaVetorLogs(vetorLogsGeral, qntLogs);
            ordenaVetorLogs(vetorLogsGeral, qntLogs);
            printf("\n");
            break;

        case 6:
            modeloEscolhido = funcaoModeloBikeSwitchCase(vetBikes, qntBikes);
            ordenaAtividadesBicicletaPorDistancia(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printaHistogramaPorBike(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printf("\n");
            break;

        default:
            printf("Digite um numero valido!\n");
            break;
        }
    }

    free(vetorLogsGeral);

    for (i = 0; i < qntBikes; i++)
        free(vetBikes[i].nome);
    free(vetBikes);

    (void)closedir(dirStream);
    return 0;
}