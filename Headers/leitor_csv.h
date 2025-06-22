#ifndef LEITOR_CSV_H
#define LEITOR_CSV_H

#include "../headers/pergunta.h"

// Remove espaços no início e fim da string (in-place)
char* trim(char *str);

// Carrega perguntas do arquivo CSV, retorna vetor dinamicamente alocado e total em *total
Pergunta* carregaPerguntasDeCSV(const char *nome_arquivo, int *total);

#endif
