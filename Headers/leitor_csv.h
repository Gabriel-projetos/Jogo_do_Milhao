#ifndef LEITOR_CSV_H
#define LEITOR_CSV_H

#include "pergunta.h"

// Remove espaços em branco no início e fim da string (in-place)
char* trim(char *str);

// Carrega perguntas de um arquivo CSV..
Pergunta* carregaPerguntasDeCSV(const char *nome_arquivo, int *total);

// Salva um array de perguntas em um arquivo CSV.
void salvaPerguntasNoCSV(Pergunta *perguntas, const char *filename, int total_perguntas);

#endif