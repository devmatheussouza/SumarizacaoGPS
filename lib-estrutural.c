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
#include <math.h>

void ordenaVetorLogs(logs *vetor, int tamanho)
{
    int k, j;
    logs aux;

    for (k = 0; k < tamanho - 1; k++){
        for (j = 0; j < tamanho - k - 1; j++){
            if (strcmp(vetor[j].nome_bicicleta, vetor[j + 1].nome_bicicleta) > 0){
                aux = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = aux;
            }
        }
    }
}

void ordenaAtividadesBicicletaPorData(logs* vetor, int tamIni, int tamFim)
{
    int i, continuaOrdenacao;
    logs aux;

    do
    {
        continuaOrdenacao = 0;
        for(i=tamIni; i<tamFim; i++){
            if(strcmp(vetor[i].data_atividade, vetor[i+1].data_atividade) > 0){
                aux = vetor[i];
                vetor[i] = vetor[i+1];
                vetor[i+1] = aux;
                continuaOrdenacao = i;
            }
        }
        tamFim--;
    } while (continuaOrdenacao != 0);
}

void ordenaAtividadesBicicletaPorDistancia(logs* vetor, int tamIni, int tamFim)
{
    int i, continuaOrdenacao;
    logs aux;

    do
    {
        continuaOrdenacao = 0;
        for(i=tamIni; i<tamFim; i++){
            if(vetor[i].distancia > vetor[i+1].distancia){
                aux = vetor[i];
                vetor[i] = vetor[i+1];
                vetor[i+1] = aux;
                continuaOrdenacao = i;
            }
        }
        tamFim--;
    } while (continuaOrdenacao != 0);
}

void ordenaAtividadesBicicletaPorSubidaAcumulada(logs* vetor, int tamIni, int tamFim)
{
    int i, continuaOrdenacao;
    logs aux;

    do
    {
        continuaOrdenacao = 0;
        for(i=tamIni; i<tamFim; i++){
            if(vetor[i].subidaAcumulada > vetor[i+1].subidaAcumulada){
                aux = vetor[i];
                vetor[i] = vetor[i+1];
                vetor[i+1] = aux;
                continuaOrdenacao = i;
            }
        }
        tamFim--;
    } while (continuaOrdenacao != 0);
}

void inicializaVetorLogs(logs *vetorLogsGeral, int tamanhoVetor)
{
    int i;
    for (i = 0; i < tamanhoVetor; i++){
        strcpy(vetorLogsGeral[i].nome_bicicleta, "z");
        strcpy(vetorLogsGeral[i].nome_arquivo, "z");
        strcpy(vetorLogsGeral[i].data_atividade, "z");
        vetorLogsGeral[i].distancia = 0;
        vetorLogsGeral[i].velocidadeMaxima = 0;
        vetorLogsGeral[i].velocidadeMedia = 0;
        vetorLogsGeral[i].heartRateMaximo = 0;
        vetorLogsGeral[i].heartRateMedio = 0;
        vetorLogsGeral[i].cadenciaMedia = 0;
        vetorLogsGeral[i].subidaAcumulada = 0;
    }
}

bikes* preencheVetorBikes(logs* vetorLogsGeral, int tamVetLogs, int* qntBikes)
{
    int i = 0, j, tamInicial = 10, qntReduzir=0;
    *qntBikes = 1;
    bikes* bicicletas = malloc(sizeof(bikes)*tamInicial);

    // Inicializa vetor de bikes;
    for(i=0; i<tamInicial; i++){
        strcpy(bicicletas[i].nome, "z");
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
        if(strcmp(vetorLogsGeral[i].nome_bicicleta, "z") == 0){
            qntReduzir++;
        }
        if(strcmp(bicicletas[j].nome, vetorLogsGeral[i].nome_bicicleta) != 0 && strcmp(vetorLogsGeral[i].nome_bicicleta, "z") != 0){
            bicicletas[j].ultimaPosicao = i-1;
            (*qntBikes)++;
            j++;
            if(j > tamInicial){
                if (!(bicicletas = (bikes*) realloc(bicicletas, 2 * sizeof(bicicletas)))){
                    perror("Nao foi possivel realocar memoria");
                    exit(1);
                }
            }
            bicicletas[j].primeiraPosicao = i;
            strcpy(bicicletas[j].nome, vetorLogsGeral[i].nome_bicicleta);
        }
        i++;
    }
    bicicletas[j].ultimaPosicao = i-1-qntReduzir;

    return bicicletas;
}

void printaVetorLogs(logs *vetorLogsGeral, int qntLogs)
{
    int i;
    printf("%-18s%-22s%-15s%-15s%-15s%-15s%-15s%-12s",
           "ARQUIVO",
           "NOME",
           "DATA",
           "DISTANCIA(KM)",
           "V. MAX(KM/H)",
           "V. MEDIA(KM/H)",
           "HR MEDIO(BPM)",
           "HR MAX(BPM)");

    printf("%-17s%-15s\n",
           "CAD. MEDIA(BPM)",
           "SUB. ACUMULADA(M)");

    i=0;
    while (i < qntLogs){
        if(strchr(vetorLogsGeral[i].nome_bicicleta, '\n') != NULL)
            vetorLogsGeral[i].nome_bicicleta[strlen(vetorLogsGeral[i].nome_bicicleta) - 1] = 0;
        
        if(strcmp(vetorLogsGeral[i].nome_bicicleta, "z") !=0){
            printf("%-18s%-22s%-15s%-15.2f%-15.2f%-15.2f%-15.0f%-12.0f%-17.0f%-15.2f\n",
                vetorLogsGeral[i].nome_arquivo,
                vetorLogsGeral[i].nome_bicicleta,
                vetorLogsGeral[i].data_atividade,
                vetorLogsGeral[i].distancia,
                vetorLogsGeral[i].velocidadeMaxima,
                vetorLogsGeral[i].velocidadeMedia,
                vetorLogsGeral[i].heartRateMedio,
                vetorLogsGeral[i].heartRateMaximo,
                vetorLogsGeral[i].cadenciaMedia,
                vetorLogsGeral[i].subidaAcumulada);
        }


        i++;
    }
}

void printAgrupadoPorBicicleta(logs *vetorLogsGeral, int tamIni, int tamFim)
{
    int i, dia, mes, ano;
    printf("Bicicleta: %s\n", vetorLogsGeral[tamIni].nome_bicicleta);
    printf("%-10s%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n",
        "DATA",
        "DISTANCIA(KM)",
        "V. MEDIA(KM/H)",
        "V. MAX(KM/H)",
        "HR MEDIO(BPM)",
        "HR MAX(BPM)",
        "CAD. MEDIA(BPM)",
        "SUB. ACUMULADA(M)");
    
    for(i=tamIni; i<tamFim; i++){
        if(strchr(vetorLogsGeral[i].nome_bicicleta, '\n') != NULL)
            vetorLogsGeral[i].nome_bicicleta[strlen(vetorLogsGeral[i].nome_bicicleta) - 1] = 0;
        sscanf(vetorLogsGeral[i].data_atividade, "%d-%d-%d", &ano, &mes, &dia);
        if(dia<10 && mes<10) printf("%d/%-8d", dia, mes);
        if(dia<10 && mes>=10) printf("%d/%-8d", dia, mes);
        if(dia>=10 && mes<10) printf("%d/%-7d", dia, mes);
        if(dia>=10 && mes>=10) printf("%d/%-7d", dia, mes);
        printf("%-20.2f%-20.2f%-20.2f%-20.0f%-20.0f%-20.0f%-20.2f\n",
               vetorLogsGeral[i].distancia,
               vetorLogsGeral[i].velocidadeMedia,
               vetorLogsGeral[i].velocidadeMaxima,
               vetorLogsGeral[i].heartRateMedio,
               vetorLogsGeral[i].heartRateMaximo,
               vetorLogsGeral[i].cadenciaMedia,
               vetorLogsGeral[i].subidaAcumulada);
    }
}

int funcaoModeloBikeSwitchCase(bikes* vetBikes, int qntBikes)
{
    int i, modeloEscolhido;
    printf("\n");
    printf("Qual modelo? \n");
    printf("\n");
    for(i=0; i<qntBikes; i++){
        // Verifica se tem '\n' como último char; Se tiver, retira;
        if(strchr(vetBikes[i].nome, '\n') != NULL){
            vetBikes[i].nome[strlen(vetBikes[i].nome) - 1] = 0;
        }
        printf("%d) %s \n", i+1, vetBikes[i].nome);
    }
    printf("\n");
    printf("Digite um valor: ");
    scanf("%d", &modeloEscolhido);
    while(modeloEscolhido < 1 || modeloEscolhido > qntBikes){
        printf("Digite um valor valido.\n");
        scanf("%d", &modeloEscolhido);
    }
    printf("\n");
    return modeloEscolhido;
}


void printaHistogramaPorBike(logs *vetorLogsGeral, int posIni, int posFim)
{
    int menorDistLog, menorDistHist, maiorDistLog, maiorDistHist, i;
    // Pega-se a menor e a maior distância e faz-se um casting para adotar um intervalo de inteiros;
    printf("Pos ini: %d Pos fim: %d\n", posIni, posFim);
    menorDistLog = (int) vetorLogsGeral[posIni].distancia;
    maiorDistLog = (int) vetorLogsGeral[posFim].distancia;
    /* Subtrai-se o último dígito para adotar um intervalo de 10 em 10; 
        Exemplo: menorDistLog = 125, entao menorDistHist = 120; */
    menorDistHist = menorDistLog - (menorDistLog % 10);
    maiorDistHist = maiorDistLog - (maiorDistLog % 10);
    printf("Menor dist: %d Maior dist: %d\n", menorDistHist, maiorDistHist);
    printf("Menor dist: %d Maior dist: %d\n", (int) vetorLogsGeral[posIni].distancia, (int) vetorLogsGeral[posFim].distancia);

    printf("Bicicleta: %s\n", vetorLogsGeral[posIni].nome_bicicleta);

    i = posIni;
    while(i <= posFim && menorDistHist <= maiorDistHist){
        if (menorDistHist >= 100){
            printf("%d - %d %s ", menorDistHist, menorDistHist+9, "|");
        } else if(menorDistHist + 10 >= 100) {
            printf("%3d - %d %2s ", menorDistHist, menorDistHist+9, "|");
        } else {
            printf("%3d - %1d %2s ", menorDistHist, menorDistHist+9, "|");
        }
        
        while(( round(vetorLogsGeral[i].distancia) >= menorDistHist) && ( round(vetorLogsGeral[i].distancia) <= (menorDistHist+9)) && i <= posFim){
            printf("* ");
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
        if(strchr(vetBikes[i].nome, '\n') != NULL){
            vetBikes[i].nome[strlen(vetBikes[i].nome) - 1] = 0;
        }
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
        // printf("Pos ini: %d  Pos fim: %d\n", i, posFim);
        if(strcmp(vetorLogsGeral[i].nome_bicicleta, "z") != 0){
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
    int i, j=0, tamIni, tamFim, ano, mes, dia;

    while(j<qntBikes){
        tamIni = vetBikes[j].primeiraPosicao;
        tamFim = vetBikes[j].ultimaPosicao;

        if(opcaoUser == 3){
            ordenaAtividadesBicicletaPorData(vetorLogsGeral, tamIni, tamFim);
        }

        if(opcaoUser == 4){
            ordenaAtividadesBicicletaPorDistancia(vetorLogsGeral, tamIni, tamFim);
        }

        printf("\n\n");
        printf("Bicicleta: %s\n\n", vetorLogsGeral[tamIni].nome_bicicleta);
        printf("%-10s%-20s%-20s%-20s%-20s%-20s%-20s%-20s\n",
            "DATA",
            "DISTANCIA(KM)",
            "V. MEDIA(KM/H)",
            "V. MAX(KM/H)",
            "HR MEDIO(BPM)",
            "HR MAX(BPM)",
            "CAD. MEDIA(BPM)",
            "SUB. ACUMULADA(M)");
        
        for(i=tamIni; i<=tamFim; i++){
            if(strchr(vetorLogsGeral[i].nome_bicicleta, '\n') != NULL)
                vetorLogsGeral[i].nome_bicicleta[strlen(vetorLogsGeral[i].nome_bicicleta) - 1] = 0;
            sscanf(vetorLogsGeral[i].data_atividade, "%d-%d-%d", &ano, &mes, &dia);
            if(dia<10 && mes<10) printf("%d/%-10d", dia, mes);
            if(dia<10 && mes>=10) printf("%d/%-10d", dia, mes);
            if(dia>=10 && mes<10) printf("%d/%-9d", dia, mes);
            if(dia>=10 && mes>=10) printf("%d/%-9d", dia, mes);
            printf("%-20.2f%-20.2f%-20.2f%-20.0f%-20.0f%-20.0f%-20.2f\n",
                vetorLogsGeral[i].distancia,
                vetorLogsGeral[i].velocidadeMedia,
                vetorLogsGeral[i].velocidadeMaxima,
                vetorLogsGeral[i].heartRateMedio,
                vetorLogsGeral[i].heartRateMaximo,
                vetorLogsGeral[i].cadenciaMedia,
                vetorLogsGeral[i].subidaAcumulada);
        }
        printf("\n\n");
        j++;
    }
}

void freeVetorLogs(logs* vetor){
    free(vetor);
}

void freeVetorBikes(bikes* vetor){
    free(vetor);
}