typedef struct
{
    char* nome;
    int primeiraPosicao;
    int ultimaPosicao;
} bikes;

typedef struct
{
    char* nome_bicicleta;
    char data_atividade[50];
    char* nome_arquivo;
    float distancia;
    float velocidadeMaxima;
    float velocidadeMedia;
    float heartRateMedio;
    float heartRateMaximo;
    float cadenciaMedia;
    float subidaAcumulada;
} logs;

void ordenadorVetLogs(logs *vetorLogsGeral, int posIni, int posFim, int opcaoUser);
void ordenaVetorLogsPorNome(logs *vetor, int tamanho);
void inicializaVetorLogs(logs *vetorLogsGeral, int tamanhoVetor);
bikes *preencheVetorBikes(logs *vetorLogsGeral, int tamVetLogs, int *qntBikes, int* tamVetBikes);
void printaSumarioPorBicicleta(logs* vetorLogsGeral, int posInicial, int posFim);
void printAgrupadoPorBicicleta(logs *vetorLogsGeral, int tamIni, int tamFim, int opcaoUser);
void printAtividadesBicicleta(logs *vetorLogsGeral, bikes* vetBikes, int qntBikes, int opcaoUser);
void printaHistogramaPorBike(logs *vetorLogsGeral, int tamIni, int tamFim);
void printaNomesBikes(bikes *vetBikes, int qntBikes);
void freeVetorLogs(logs* vetor, int qntLogs);
void freeVetorBikes(bikes* vetor, int tamVetBikes);