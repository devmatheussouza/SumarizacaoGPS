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

#define NUM_COMMANDS 8

// Similar a funcao do histograma, mas aqui procura qual intervalo teve maior quantidade;
int maiorQntEmDeterminadoIntervaloDist(logs* vetorLogsGeral, int posIni, int posFim){
    int i, menorDistLog, menorDistHist, maiorDistLog, maiorDistHist,  qnt, maiorQnt=0;

    menorDistLog = (int) vetorLogsGeral[posIni].distancia;
    maiorDistLog = (int) vetorLogsGeral[posFim].distancia;

    menorDistHist = menorDistLog - (menorDistLog % 10);
    maiorDistHist = maiorDistLog - (maiorDistLog % 10);

    i = posIni;
    while(i <= posFim && menorDistHist <= maiorDistHist){
        qnt = 0;
        while((round(vetorLogsGeral[i].distancia) >= menorDistHist) && (round(vetorLogsGeral[i].distancia) <= (menorDistHist+9)) && i <= posFim){
            qnt++;
            if(i == posFim) break;
            i++;
        }
        if(maiorQnt < qnt) maiorQnt = qnt;
        menorDistHist += 10;
    }

    return maiorQnt;
}


void plotaGrafico(logs* vetorLogsGeral, int posIni, int posFim){
    int i, menorDistLog, menorDistHist, maiorDistLog, maiorDistHist,  qnt, maiorQnt;
    char commandsPopen[NUM_COMMANDS][200] = {"set title \"Histograma\"", 
    "set xlabel \"Quantidade\"",
    "set ylabel \"Distância(KM)\"",
    "set yrange [0:*] reverse",
    "set style fill solid", // Adiciona cor à barra;
    "unset key", // Não printa as configurações usadas para plotar o gráfico;
    "set offsets 0,0,0.2,0.5",  // espaços vazios left, right, top, bottom;
     "plot '-' using (column(2)):(column(0)):(0):(column(2)):($0-0.3):($0+0.3):ytic(1) with boxxyerror lc 1"};
     // plot '-': plotar na linha de comando do gnuplot; 
     // columns referem-se aos dados lançados diretamente na linha de comando;
     /* column(0): refere-se ao centro da barra no eixo y.
        Além disso, gera números crescentes (a partir do zero) em cada ponto do gráfico; */
     // column(1): intervalos de distância (eixo y);
     // column(2): quantidade de distâncias dentro do intervalo (eixo x);
     // (0): origem do gráfico;
     // ($0-0.3) e ($0+0.3) referem-se à largura da barra (tem como referência o centro da barra no eixo y);
     // ytic(1) pega os dados da primeira coluna e utiliza como labels no eixo y (20-29, por exmeplo);

    // executa os comandos de dentro no programa em C direto no terminal (interface gnuplot);
    // A opção persist diz ao gnuplot para deixar as janelas dos graficos abertas quando finalizar o programa.
    FILE * linhaComandoPlot = popen ("gnuplot -persist", "w");

    // maior quantidade de distancias de um intervalo em uma bicicleta especifica;
    maiorQnt = maiorQntEmDeterminadoIntervaloDist(vetorLogsGeral, posIni, posFim);
    
    // set limite do eixo x no grafico de acordo com a maior quantidade encontrada;
    fprintf(linhaComandoPlot, "set xrange [0:%d]\n", maiorQnt+1);

    // joga as configuracoes do plot direto no terminal;
    for (i=0; i < NUM_COMMANDS; i++)
        fprintf(linhaComandoPlot, "%s\n", commandsPopen[i]); 

    
    menorDistLog = (int) vetorLogsGeral[posIni].distancia;
    maiorDistLog = (int) vetorLogsGeral[posFim].distancia;

    menorDistHist = menorDistLog - (menorDistLog % 10);
    maiorDistHist = maiorDistLog - (maiorDistLog % 10);

    i = posIni;
    while(i <= posFim && menorDistHist <= maiorDistHist){
        qnt = 0;
        while((round(vetorLogsGeral[i].distancia) >= menorDistHist) && (round(vetorLogsGeral[i].distancia) <= (menorDistHist+9)) && i <= posFim){
            qnt++;
            if(i == posFim) break;
            i++;
        }
        // escreve no terminal os dados para plotar o grafico;
        // Coluna 1 -> intervalo das distancias;
        // Coluna 2 -> quantidade de distancias nesse intervalo;
        fprintf(linhaComandoPlot, "%d-%d\t%d\n", menorDistHist, menorDistHist+9, qnt);
        menorDistHist += 10;
    }

    // finaliza a adicao de comandos no terminal e plota o grafico;
    fprintf(linhaComandoPlot, "e\n");

    // fecha o "arquivo";
    fclose(linhaComandoPlot);
}