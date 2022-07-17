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
#include "plota-grafico.h"

#define NOME_INI "~"

void inicializaVetorLogs(logs *vetorLogsGeral, int tamanhoVetor)
{
    int i;
    for (i = 0; i < tamanhoVetor; i++){
        if (!(vetorLogsGeral[i].nome_bicicleta = malloc(sizeof(char)*1000))){
            perror("Nao foi possivel alocar memoria");
            exit(1);
        }
        if (!(vetorLogsGeral[i].nome_arquivo = malloc(sizeof(char)*1000))){
            perror("Nao foi possivel alocar memoria");
            exit(1);
        }
        strcpy(vetorLogsGeral[i].nome_bicicleta, NOME_INI);
        strcpy(vetorLogsGeral[i].nome_arquivo, NOME_INI);
        strcpy(vetorLogsGeral[i].data_atividade, NOME_INI);
        vetorLogsGeral[i].distancia = 0;
        vetorLogsGeral[i].velocidadeMaxima = 0;
        vetorLogsGeral[i].velocidadeMedia = 0;
        vetorLogsGeral[i].heartRateMaximo = 0;
        vetorLogsGeral[i].heartRateMedio = 0;
        vetorLogsGeral[i].cadenciaMedia = 0;
        vetorLogsGeral[i].subidaAcumulada = 0;
    }
}

void ordenaVetorLogsPorNome(logs *vetor, int tamanho)
{
    int i, j;
    logs aux;

    for (i = 0; i < tamanho - 1; i++){
        for (j = 0; j < tamanho - i - 1; j++){
            if (strcmp(vetor[j].nome_bicicleta, vetor[j + 1].nome_bicicleta) > 0){
                aux = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = aux;
            }
        }
    }
}

void ordenadorVetLogs(logs* vetor, int posIni, int posFim, int opcaoUser)
{
    int i, j, indexMenor;
    logs aux;

    if(opcaoUser == 3){
        for(i=posIni; i<posFim; i++){
            indexMenor = i;
            for(j=i+1; j<=posFim; j++){
                if(strcmp(vetor[j].data_atividade, vetor[indexMenor].data_atividade) < 0) 
                    indexMenor = j;
            }
            if(i != indexMenor){
                aux = vetor[i];
                vetor[i] = vetor[indexMenor];
                vetor[indexMenor] = aux;
            }
        }
    }

    if(opcaoUser == 4 || opcaoUser == 6 || opcaoUser == 7){
        for(i=posIni; i<posFim; i++){
            indexMenor = i;
            for(j=i+1; j<=posFim; j++){
                if(vetor[j].distancia < vetor[indexMenor].distancia) 
                    indexMenor = j;
            }
            if(i != indexMenor){
                aux = vetor[i];
                vetor[i] = vetor[indexMenor];
                vetor[indexMenor] = aux;
            }
        }
    }

    if(opcaoUser == 5){
        for(i=posIni; i<posFim; i++){
            indexMenor = i;
            for(j=i+1; j<=posFim; j++){
                if(vetor[j].subidaAcumulada < vetor[indexMenor].subidaAcumulada) 
                    indexMenor = j;
            }
            if(i != indexMenor){
                aux = vetor[i];
                vetor[i] = vetor[indexMenor];
                vetor[indexMenor] = aux;
            }
        }
    }
}

bikes* preencheVetorBikes(logs* vetorLogsGeral, int tamVetLogs, int* qntBikes, int* tamVetBikes)
{
    int i = 0, j, qntReduzir=0;
    *tamVetBikes = 1;
    *qntBikes = 1;
    bikes* bicicletas = malloc(sizeof(bikes)*(*tamVetBikes));

    // Inicializa vetor de bikes;
    for(i=0; i<*tamVetBikes; i++){
        if (!(bicicletas[i].nome = malloc(sizeof(char) * 1000))){
            perror("Nao foi possivel alocar memoria");
            exit(1);
        }
        strcpy(bicicletas[i].nome, NOME_INI);
        bicicletas[i].primeiraPosicao = -1;
        bicicletas[i].ultimaPosicao = -1;
    }

    strcpy(bicicletas[0].nome, vetorLogsGeral[0].nome_bicicleta);
    bicicletas[0].primeiraPosicao = 0;
    bicicletas[0].ultimaPosicao = 0;

    i=0, j=0;
    /* Verifica quantas bikes têm no vetor de logs e preenche o vetor de bikes com nome e 
        posicão inicial e final (no vetor de logs) de determinada bicicleta */
    while(i < tamVetLogs){
        if(strcmp(vetorLogsGeral[i].nome_bicicleta, NOME_INI) == 0){
            qntReduzir++;
        }
        if(strcmp(bicicletas[j].nome, vetorLogsGeral[i].nome_bicicleta) != 0 && strcmp(vetorLogsGeral[i].nome_bicicleta, NOME_INI) != 0){
            bicicletas[j].ultimaPosicao = i-1;
            (*qntBikes)++;
            j++;
            if(j >= *tamVetBikes){
                (*tamVetBikes) += 1;
                if (!(bicicletas = (bikes*) realloc(bicicletas, *tamVetBikes * sizeof(bikes)))){
                    perror("Nao foi possivel realocar memoria");
                    exit(1);
                }
                if (!(bicicletas[j].nome = malloc(sizeof(char) * 1000))){
                    perror("Nao foi possivel alocar memoria");
                    exit(1);
                }
                strcpy(bicicletas[j].nome, NOME_INI);
                bicicletas[j].primeiraPosicao = -1;
                bicicletas[j].ultimaPosicao = -1;
            }
            bicicletas[j].primeiraPosicao = i;
            if(sizeof(vetorLogsGeral[i].nome_bicicleta) > sizeof(bicicletas[i].nome)){
                if (!(bicicletas[i].nome = (char*) realloc(bicicletas[i].nome, 2 * sizeof(bicicletas[i].nome)))){
                    perror("Nao foi possivel realocar memoria");
                    exit(1);
                }
            }
            strcpy(bicicletas[j].nome, vetorLogsGeral[i].nome_bicicleta);
        }
        i++;
    }
    bicicletas[j].ultimaPosicao = i-1-qntReduzir;
    return bicicletas;
}

void printAgrupadoPorBicicleta(logs *vetorLogsGeral, int tamIni, int tamFim, int opcaoUser)
{
    int i, dia, mes, ano;
    if(opcaoUser == 5){
        printf("\nLista de todas atividades ordenadas por subida acumulada\n\n");
        printf("%-22s", "BICICLETA");
    } else {
        printf("Bicicleta: %s\n", vetorLogsGeral[tamIni].nome_bicicleta);
    }
    printf("%-10s%-15s%-15s%-15s%-15s%-15s%-17s%-15s\n",
        "DATA",
        "DISTANCIA(KM)",
        "V. MEDIA(KM/H)",
        "V. MAX(KM/H)",
        "HR MEDIO(BPM)",
        "HR MAX(BPM)",
        "CAD. MEDIA(RPM)",
        "SUB. ACUMULADA(M)");
    
    for(i=tamIni; i<=tamFim; i++){
        if(strcmp(vetorLogsGeral[i].nome_bicicleta, NOME_INI) !=0){
            if(strchr(vetorLogsGeral[i].nome_bicicleta, '\n') != NULL)
                vetorLogsGeral[i].nome_bicicleta[strlen(vetorLogsGeral[i].nome_bicicleta) - 1] = 0;
            if(opcaoUser==5) printf("%-22s", vetorLogsGeral[i].nome_bicicleta);
            if(strcmp(vetorLogsGeral[i].data_atividade, NOME_INI)==0){
                printf("%-10s", "~~~~");
            } else {
                sscanf(vetorLogsGeral[i].data_atividade, "%d-%d-%d", &ano, &mes, &dia);
                if(dia<10 && mes<10) printf("%02d/%02d%5s", dia, mes, "");
                if(dia<10 && mes>=10) printf("%02d/%02d%5s", dia, mes, "");
                if(dia>=10 && mes<10) printf("%02d/%02d%5s", dia, mes, "");
                if(dia>=10 && mes>=10) printf("%02d/%02d%5s", dia, mes, "");
            }
            printf("%-15.2f%-15.2f%-15.2f%-15.0f%-15.0f%-17.0f%-15.2f\n",
                vetorLogsGeral[i].distancia,
                vetorLogsGeral[i].velocidadeMedia,
                vetorLogsGeral[i].velocidadeMaxima,
                vetorLogsGeral[i].heartRateMedio,
                vetorLogsGeral[i].heartRateMaximo,
                vetorLogsGeral[i].cadenciaMedia,
                vetorLogsGeral[i].subidaAcumulada);
        }
    }
}

void printaHistogramaPorBike(logs *vetorLogsGeral, int posIni, int posFim)
{
    int menorDistLog, menorDistHist, maiorDistLog, maiorDistHist, i;
    // Pega-se a menor e a maior distância e faz-se um casting para adotar um intervalo de inteiros;
    menorDistLog = (int) vetorLogsGeral[posIni].distancia;
    maiorDistLog = (int) vetorLogsGeral[posFim].distancia;
    /* Subtrai-se o último dígito para adotar um intervalo de 10 em 10; 
        Exemplo: menorDistLog = 125, entao menorDistHist = 120; */
    menorDistHist = menorDistLog - (menorDistLog % 10);
    maiorDistHist = maiorDistLog - (maiorDistLog % 10);

    printf("Bicicleta: %s\n", vetorLogsGeral[posIni].nome_bicicleta);

    i = posIni;
    while(i <= posFim && menorDistHist <= maiorDistHist){
        if (menorDistHist >= 100) printf("%d - %d %s ", menorDistHist, menorDistHist+9, "|");
        else if(menorDistHist + 10 >= 100) printf("%3d - %d %2s ", menorDistHist, menorDistHist+9, "|");
        else printf("%3d - %1d %2s ", menorDistHist, menorDistHist+9, "|");
        
        while(( round(vetorLogsGeral[i].distancia) >= menorDistHist) && ( round(vetorLogsGeral[i].distancia) <= (menorDistHist+9)) && i <= posFim){
            printf("* ");
            if(i == posFim) break;
            i++;
        }
        menorDistHist += 10;
        printf("\n");
    }
    printf("%-10s%s%11s\n", "DISTANCIA", "|", "QUANTIDADE");
}

void printaNomesBikes(bikes *vetBikes, int qntBikes)
{
    int i;
    printf("\n");
    printf("*** Modelos encontrados ***\n");
    printf("\n");
    for(i=0; i<qntBikes; i++){
        if(strchr(vetBikes[i].nome, '\n') != NULL)
            vetBikes[i].nome[strlen(vetBikes[i].nome) - 1] = 0;
        printf("%s \n", vetBikes[i].nome);
    }
    printf("\n");
    printf("***************************\n");
    printf("\n");
}

void printaSumarioPorBicicleta(logs *vetorLogsGeral, int posInicial, int posFim)
{
    int i, qntDist=0;
    float somaDist=0, menorDist=vetorLogsGeral[posInicial].distancia, maiorDist=vetorLogsGeral[posInicial].distancia, distMedia=0;
    for(i=posInicial; i<=posFim; i++){
        if(strcmp(vetorLogsGeral[i].nome_bicicleta, NOME_INI) != 0){
            somaDist += vetorLogsGeral[i].distancia;
            if(menorDist > vetorLogsGeral[i].distancia)
                menorDist = vetorLogsGeral[i].distancia;
            if(maiorDist < vetorLogsGeral[i].distancia)
                maiorDist = vetorLogsGeral[i].distancia;
            qntDist++;
        }
    }

    distMedia = somaDist/(posFim-posInicial+1);

    printf("\n");
    printf("******** Sumario dos logs ********\n");
    printf("\n");
    printf("Quantidade de atividades: %d\n", qntDist);
    printf("Total percorrido: %.2f km\n", somaDist);
    printf("Pedal mais longo: %.2f km\n", maiorDist);
    printf("Pedal mais curto: %.2f km\n", menorDist);
    printf("Distancia media: %6.2f km\n", distMedia);
    printf("\n");
    printf("**********************************\n");
    printf("\n");
}

void printAtividadesBicicleta(logs *vetorLogsGeral, bikes* vetBikes, int qntBikes, int opcaoUser)
{
    int j=0, tamIni, tamFim;

    while(j<qntBikes){
        tamIni = vetBikes[j].primeiraPosicao;
        tamFim = vetBikes[j].ultimaPosicao;

        if(opcaoUser == 3) ordenadorVetLogs(vetorLogsGeral, tamIni, tamFim, opcaoUser);
        if(opcaoUser == 4) ordenadorVetLogs(vetorLogsGeral, tamIni, tamFim, opcaoUser);

        printf("\n\n");
        printAgrupadoPorBicicleta(vetorLogsGeral, tamIni, tamFim, opcaoUser);
        printf("\n\n");
        j++;
    }
}

void freeVetorLogs(logs* vetor, int qntLogs){
    int i;
    for(i=0; i<qntLogs; i++){
        free(vetor[i].nome_bicicleta);
        free(vetor[i].nome_arquivo);
    }
    free(vetor);
}

void freeVetorBikes(bikes* vetor, int qntBikes){
    int i;
    for(i=0; i<qntBikes; i++){
        free(vetor[i].nome);
    }
    free(vetor);
}