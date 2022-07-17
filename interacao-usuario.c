#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include "interacao-usuario.h"

void interacaoResultadosComUsuario(logs* vetorLogsGeral, bikes* vetBikes, int qntBikes, int qntLogs)
{
    int opcaoUser, modeloEscolhido;
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
        printf("7) Escolher um modelo de bike e plotar um grafico da distribuicao das distancias\n");
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
            printAgrupadoPorBicicleta(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao, opcaoUser);
            printaSumarioPorBicicleta(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printf("\n");
            break;

        case 3:
            printAtividadesBicicleta(vetorLogsGeral, vetBikes, qntBikes, opcaoUser);
            printf("\n");
            break;

        case 4:
            printAtividadesBicicleta(vetorLogsGeral, vetBikes, qntBikes, opcaoUser);
            printf("\n");
            break;

        case 5:
            ordenadorVetLogs(vetorLogsGeral, 0, qntLogs-1, opcaoUser);
            printAgrupadoPorBicicleta(vetorLogsGeral, 0, qntLogs-1, opcaoUser);
            ordenaVetorLogsPorNome(vetorLogsGeral, qntLogs);
            printf("\n");
            break;

        case 6:
            modeloEscolhido = funcaoModeloBikeSwitchCase(vetBikes, qntBikes);
            ordenadorVetLogs(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao, opcaoUser);
            printaHistogramaPorBike(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printf("\n");
            break;

        case 7: 
            modeloEscolhido = funcaoModeloBikeSwitchCase(vetBikes, qntBikes);
            ordenadorVetLogs(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao, opcaoUser);
            plotaGrafico(vetorLogsGeral, vetBikes[modeloEscolhido - 1].primeiraPosicao, vetBikes[modeloEscolhido - 1].ultimaPosicao);
            printf("\n");

        default:
            printf("Digite um numero valido!\n");
            break;
        }
    }
}