#include <dirent.h>

int diferencaEntreDoisTimestamps(char *timestampAnterior, char *timestampAtual);
int verificaQntArquivosEmDiretorio(DIR *dirStream, char *caminhoDiretorio);
int isDir(const char *fileName);