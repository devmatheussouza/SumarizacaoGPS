#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "lib-estrutural.h"
#include "funcoes-auxiliares.h"

#define LINESIZE 1024

int main(int argc, char **argv)
{
    char line[LINESIZE + 1];
    DIR *dirStream;
    struct dirent *dirEntry;
    char *caminho = argv[2];
    FILE *arq;
    char *nomeDoArquivo, *caminhoMaisNomeArquivo, *ptr_string;
    logs *vetorLogsGeral;
    bikes *vetBikes;
    char timestampAnterior[20];
    int qntBikes, i, qntLogs, opcaoUser, modeloEscolhido;
    int qntCD, qntV, qntHR;
    int difEntreTempos;
    float resCD, resCD_At, resHR, resHR_At, somaHR, somaCD;
    float primeiraDist, altitudeAnterior, resultado, somaV, resV, resV_At;

    if (caminho[strlen(caminho) - 1] != '/')
        strcat(caminho, "/");

    if (!(dirStream = opendir(caminho))){
        perror("Nao foi possivel abrir o diretorio.");
        exit(1);
    }

    if (!(caminhoMaisNomeArquivo = malloc(sizeof(char) * 1000))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    if (!(nomeDoArquivo = malloc(sizeof(char) * 1000))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    printf("Realizando leitura dos arquivos de log...\n");

    qntLogs = verificaQntArquivosEmDiretorio(dirStream, caminho);
    
    if (!(vetorLogsGeral = malloc(sizeof(logs) * qntLogs))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    inicializaVetorLogs(vetorLogsGeral, qntLogs);

    i = 0;
    for (;;){
        if (!(dirEntry = readdir(dirStream)))
            break;

        strcpy(caminhoMaisNomeArquivo, caminho);
        strcpy(nomeDoArquivo, dirEntry->d_name);
        strcat(caminhoMaisNomeArquivo, nomeDoArquivo);

        if (isDir(caminhoMaisNomeArquivo) != 0){
            arq = fopen(caminhoMaisNomeArquivo, "r");
            if (!arq){
                perror("Erro ao abrir arquivo.\n");
                exit(1);
            }

            strcpy(vetorLogsGeral[i].nome_arquivo, nomeDoArquivo);

            strcpy(timestampAnterior, "z");
            primeiraDist = 0;
            altitudeAnterior = 0;
            qntV = 0;
            qntHR = 0;
            qntCD = 0;
            somaHR = 0;
            somaV = 0;
            somaCD = 0;
            resCD = -1, resCD_At = 0;
            resHR = -1, resHR_At = 0;
            resV = -1, resV_At = 0;

            while (fgets(line, LINESIZE, arq) != NULL){
                ptr_string = strtok(line, ":");

                if (strcmp(ptr_string, "Gear") == 0){
                    ptr_string = strtok(NULL, "");
                    strcpy(vetorLogsGeral[i].nome_bicicleta, ptr_string + 1);
                    continue;
                }

                if (strcmp(vetorLogsGeral[i].nome_bicicleta, "z") == 0){
                    qntLogs--;
                    break;
                }

                if (strcmp(ptr_string, "Date") == 0 && strcmp(vetorLogsGeral[i].data_atividade, "z") == 0){
                    ptr_string = strtok(NULL, " ");
                    ptr_string = strtok(NULL, " ");
                    ptr_string = strtok(NULL, " ");
                    ptr_string = strtok(NULL, " ");
                    strcpy(timestampAnterior, ptr_string);
                    continue;
                }

                if (strcmp(ptr_string, "altitude") == 0 && altitudeAnterior < 0.1){
                    ptr_string = strtok(NULL, " ");
                    resultado = atof(ptr_string);
                    altitudeAnterior = resultado;
                    continue;
                }

                if (strcmp(ptr_string, "altitude") == 0){
                    ptr_string = strtok(NULL, " ");
                    resultado = atof(ptr_string);
                    if (resultado > altitudeAnterior)
                        vetorLogsGeral[i].subidaAcumulada += resultado - altitudeAnterior;
                    altitudeAnterior = resultado;
                    continue;
                }

                if (strcmp(ptr_string, "cadence") == 0 && resCD < 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0)
                        resCD = 0;
                    else
                        resCD = atof(ptr_string);
                    continue;
                }

                if (strcmp(ptr_string, "cadence") == 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0)
                        resCD_At = 0;
                    else
                        resCD_At = atof(ptr_string);
                    continue;
                }

                if (strcmp(ptr_string, "distance") == 0 && primeiraDist < 0.1){
                    ptr_string = strtok(NULL, " ");
                    primeiraDist = atof(ptr_string);
                    continue;
                }

                if (strcmp(ptr_string, "distance") == 0){
                    ptr_string = strtok(NULL, " ");
                    vetorLogsGeral[i].distancia = atof(ptr_string);
                    continue;
                }

                if (strcmp(ptr_string, "heart_rate") == 0 && resHR < 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0)
                        resHR = 0;
                    else
                        resHR = atof(ptr_string);

                    if (vetorLogsGeral[i].heartRateMaximo < resHR)
                        vetorLogsGeral[i].heartRateMaximo = resHR;
                    continue;
                }

                if (strcmp(ptr_string, "heart_rate") == 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0)
                        resHR_At = 0;
                    else
                        resHR_At = atof(ptr_string);

                    if (vetorLogsGeral[i].heartRateMaximo < resHR)
                        vetorLogsGeral[i].heartRateMaximo = resHR;
                    continue;
                }

                if (strcmp(ptr_string, "speed") == 0 && resV < 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0){
                        resV = 0;
                    } else {
                        resV = atof(ptr_string) * 3.6;
                        if (vetorLogsGeral[i].velocidadeMaxima < resV)
                            vetorLogsGeral[i].velocidadeMaxima = resV;
                    }
                    continue;
                }

                if (strcmp(ptr_string, "speed") == 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0){
                        resV_At = 0;
                    } else {
                        resV_At = atof(ptr_string) * 3.6;
                        if (vetorLogsGeral[i].velocidadeMaxima < resV)
                            vetorLogsGeral[i].velocidadeMaxima = resV;
                    }
                    continue;
                }

                if (strcmp(ptr_string, "timestamp") == 0 && (strcmp(vetorLogsGeral[i].data_atividade, "z") == 0)){
                    ptr_string = strtok(NULL, " ");
                    strcpy(vetorLogsGeral[i].data_atividade, ptr_string);
                    continue;
                }

                if (strcmp(ptr_string, "timestamp") == 0){
                    ptr_string = strtok(NULL, " ");
                    ptr_string = strtok(NULL, " ");
                    difEntreTempos = diferencaEntreDoisTimestamps(timestampAnterior, ptr_string);

                    if (difEntreTempos > 1 && resCD > 0){
                        qntCD += difEntreTempos;
                        somaCD += resCD * difEntreTempos;
                    } else if (difEntreTempos == 1 && resCD > 0) {
                        qntCD++;
                        somaCD += resCD;
                    }

                    if (difEntreTempos > 1 && resHR > 0){
                        qntHR += difEntreTempos;
                        somaHR += resHR * difEntreTempos;
                    } else if (difEntreTempos == 1 && resHR > 0) {
                        qntHR++;
                        somaHR += resHR;
                    }

                    if (difEntreTempos > 1 && resV > 0){
                        qntV += difEntreTempos;
                        somaV += resV * difEntreTempos;
                    } else if (difEntreTempos == 1 && resV > 0){
                        qntV++;
                        somaV += resV;
                    }

                    strcpy(timestampAnterior, ptr_string);
                    resCD = resCD_At;
                    resHR = resHR_At;
                    resV = resV_At;
                    continue;
                }
            }

            if (qntV > 0)
                vetorLogsGeral[i].velocidadeMedia = somaV / qntV;
            if (qntHR > 0)
                vetorLogsGeral[i].heartRateMedio = somaHR / qntHR;
            if (qntCD > 0)
                vetorLogsGeral[i].cadenciaMedia = somaCD / qntCD;

            vetorLogsGeral[i].distancia -= primeiraDist;
            vetorLogsGeral[i].distancia = vetorLogsGeral[i].distancia / 1000;
            i++;

            fclose(arq);
        }
    }

    ordenaVetorLogs(vetorLogsGeral, qntLogs);

    // printf("QUANTIDADE DE LOGS: %d\n", qntLogs);

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

    free(caminhoMaisNomeArquivo);
    free(nomeDoArquivo);
    free(vetorLogsGeral);

    for (i = 0; i < qntBikes; i++)
        free(vetBikes[i].nome);
    free(vetBikes);

    (void)closedir(dirStream);
    return 0;
}