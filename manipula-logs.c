#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "interacao-usuario.h"

#define NOME_INI "~"
#define LINESIZE 1024

void manipulaArquivosLog(DIR* dirStream, char* caminho, logs* vetorLogsGeral, int qntLogs)
{
    char line[LINESIZE + 1];
    struct dirent *dirEntry;
    FILE *arq;
    char *nomeDoArquivo, *caminhoMaisNomeArquivo, *ptr_string;
    char timestampAnterior[20];
    int qntCD, qntV, qntHR, i;
    int difEntreTempos;
    float resCD, resCD_At, resHR, resHR_At, somaHR, somaCD;
    float primeiraDist, altitudeAnterior, resultado, somaV, resV, resV_At;

    if (!(caminhoMaisNomeArquivo = malloc(sizeof(char) * 1000))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    if (!(nomeDoArquivo = malloc(sizeof(char) * 1000))){
        perror("Nao foi possivel alocar memoria");
        exit(1);
    }

    i = 0;
    for (;;){
        // Se não tiver mais entradas, break;
        if (!(dirEntry = readdir(dirStream))){
            printf("Leitura finalizada! [100%s]\n\n", "%");
            break;
        } 

        strcpy(caminhoMaisNomeArquivo, caminho);
        strcpy(nomeDoArquivo, dirEntry->d_name);
        // Concatena caminho do diretório com o nome do arquivo;
        strcat(caminhoMaisNomeArquivo, nomeDoArquivo);

        // Se for arquivo, entra no escopo do if;
        if (isDir(caminhoMaisNomeArquivo) != 0){
            if((((float) i) / ((float) qntLogs))*100 >= 100){
                printf("Finalizando leitura...\n");
            } else {
                printf("Lendo arquivos de log, aguarde... [%.2f%s]\n", (((float) i) / ((float) qntLogs))*100, "%");
            }
            arq = fopen(caminhoMaisNomeArquivo, "r");
            if (!arq){
                perror("Erro ao abrir arquivo.\n");
                exit(1);
            }

            strcpy(vetorLogsGeral[i].nome_arquivo, nomeDoArquivo);

            strcpy(timestampAnterior, NOME_INI);
            primeiraDist = 0;
            altitudeAnterior = 0;
            qntV = 0, qntHR = 0, qntCD = 0;
            somaHR = 0, somaV = 0, somaCD = 0;
            resCD = -1, resCD_At = 0;
            resHR = -1, resHR_At = 0;
            resV = -1, resV_At = 0;

            while (fgets(line, LINESIZE, arq) != NULL){
                ptr_string = strtok(line, ":");

                if (strcmp(ptr_string, "Gear") == 0){
                    ptr_string = strtok(NULL, "");
                    if(sizeof(ptr_string) > sizeof(vetorLogsGeral[i].nome_bicicleta)){
                        if (!(vetorLogsGeral[i].nome_bicicleta = (char*) realloc(vetorLogsGeral[i].nome_bicicleta, 2 * sizeof(vetorLogsGeral[i].nome_bicicleta)))){
                            perror("Nao foi possivel realocar memoria");
                            exit(1);
                        }
                    }
                    strcpy(vetorLogsGeral[i].nome_bicicleta, ptr_string + 1);
                    continue;
                }

                /* Se passar da parte do Gear sem ter atribuído o nome da bike,
                    é realizado um break, pois não é um arquivo de log */
                if (strcmp(vetorLogsGeral[i].nome_bicicleta, NOME_INI) == 0) break;

                /* Cópia do primeiro horário do arquivo de log */
                if (strcmp(ptr_string, "Date") == 0 && strcmp(vetorLogsGeral[i].data_atividade, NOME_INI) == 0){
                    ptr_string = strtok(NULL, " ");
                    ptr_string = strtok(NULL, " ");
                    ptr_string = strtok(NULL, " ");
                    ptr_string = strtok(NULL, " ");
                    strcpy(timestampAnterior, ptr_string);
                    continue;
                }

                // Atribuição da primeira altitude
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

                // Atribuição da primeira cadência
                if (strcmp(ptr_string, "cadence") == 0 && resCD < 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0 || strcmp(ptr_string, " ") == 0)
                        resCD = 0;
                    else
                        resCD = atof(ptr_string);
                    continue;
                }

                if (strcmp(ptr_string, "cadence") == 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0 || strcmp(ptr_string, " ") == 0)
                        resCD_At = 0;
                    else
                        resCD_At = atof(ptr_string);
                    continue;
                }

                // Atribuição da primeira distância
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

                // Atribuição do primeiro HR
                if (strcmp(ptr_string, "heart_rate") == 0 && resHR < 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0 || strcmp(ptr_string, " ") == 0)
                        resHR = 0;
                    else
                        resHR = atof(ptr_string);

                    if (vetorLogsGeral[i].heartRateMaximo < resHR)
                        vetorLogsGeral[i].heartRateMaximo = resHR;
                    continue;
                }

                if (strcmp(ptr_string, "heart_rate") == 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0 || strcmp(ptr_string, " ") == 0)
                        resHR_At = 0;
                    else
                        resHR_At = atof(ptr_string);

                    if (vetorLogsGeral[i].heartRateMaximo < resHR)
                        vetorLogsGeral[i].heartRateMaximo = resHR;
                    continue;
                }

                // Atribuição da primeira velocidade
                if (strcmp(ptr_string, "speed") == 0 && resV < 0){
                    ptr_string = strtok(NULL, " ");
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0 || strcmp(ptr_string, " ") == 0){
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
                    if (strcmp(ptr_string, "None") == 0 || strcmp(ptr_string, "") == 0 || strcmp(ptr_string, " ") == 0){
                        resV_At = 0;
                    } else {
                        resV_At = atof(ptr_string) * 3.6;
                        if (vetorLogsGeral[i].velocidadeMaxima < resV)
                            vetorLogsGeral[i].velocidadeMaxima = resV;
                    }
                    continue;
                }

                // Cópia da data do log
                if (strcmp(ptr_string, "timestamp") == 0 && (strcmp(vetorLogsGeral[i].data_atividade, NOME_INI) == 0)){
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
                }
            }

            if (qntV > 0) vetorLogsGeral[i].velocidadeMedia = somaV / qntV;
            if (qntHR > 0) vetorLogsGeral[i].heartRateMedio = somaHR / qntHR;
            if (qntCD > 0) vetorLogsGeral[i].cadenciaMedia = somaCD / qntCD;

            vetorLogsGeral[i].distancia -= primeiraDist;
            vetorLogsGeral[i].distancia = vetorLogsGeral[i].distancia / 1000;
            i++;

            fclose(arq);
        }
    }

    free(caminhoMaisNomeArquivo);
    free(nomeDoArquivo);

}
