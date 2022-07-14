#include "lib-estrutural.h"
#include <dirent.h>

void manipulaArquivosLog(DIR* dirStream, char* caminho, logs* vetorLogsGeral, int* qntLogs);
void interacaoResultadosComUsuario(logs* vetorLogsGeral, bikes* vetBikes, int qntBikes, int qntLogs);