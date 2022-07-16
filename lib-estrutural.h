typedef struct
{
    char nome[100];
    int primeiraPosicao;
    int ultimaPosicao;
} bikes;

typedef struct
{
    char nome_bicicleta[100];
    char data_atividade[100];
    char nome_arquivo[100];
    float distancia;
    float velocidadeMaxima;
    float velocidadeMedia;
    float heartRateMedio;
    float heartRateMaximo;
    float cadenciaMedia;
    float subidaAcumulada;
} logs;

void ordenaVetorLogs(logs *vetor, int tamanho);
void ordenaAtividadesBicicletaPorData(logs *vetor, int tamIni, int tamFim);
void ordenaAtividadesBicicletaPorDistancia(logs *vetor, int tamIni, int tamFim);
void ordenaAtividadesBicicletaPorSubidaAcumulada(logs *vetor, int tamIni, int tamFim);
void inicializaVetorLogs(logs *vetorLogsGeral, int tamanhoVetor);
bikes *preencheVetorBikes(logs *vetorLogsGeral, int tamVetLogs, int *qntBikes);
int funcaoModeloBikeSwitchCase(bikes *vetBikes, int qntBikes);
void printaSumarioPorBicicleta(logs* vetorLogsGeral, int posInicial, int posFim);
void printaVetorLogs(logs *vetorLogsGeral, int qntLogs);
void printAgrupadoPorBicicleta(logs *vetorLogsGeral, int tamIni, int tamFim, int opcaoUser);
void printAtividadesBicicleta(logs *vetorLogsGeral, bikes* vetBikes, int qntBikes, int opcaoUser);
void printaHistogramaPorBike(logs *vetorLogsGeral, int tamIni, int tamFim);
void printaNomesBikes(bikes *vetBikes, int qntBikes);
void plotaGrafico(logs* vetorLogsGeral, int posIni, int posFim);
int maiorQntEmDeterminadoIntervaloDist(logs* vetorLogsGeral, int posIni, int posFim);
void freeVetorLogs(logs* vetor);
void freeVetorBikes(bikes* vetor);