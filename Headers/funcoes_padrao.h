#ifndef FUNCOES_PADRAO_H
#define FUNCOES_PADRAO_H

#include "pergunta.h"

// Lê uma linha de texto de tamanho arbitrário do stdin, retornando string alocada
char *leTextoDinamico();

// Limpa o buffer do stdin
void limpaBuffer(void);

// Lê texto de tamanho fixo (máximo tamanho-1), remove '\n'
void letexto(char texto[], int tamanho);

// Aloca vetor de inteiros
int* alocaVetor(int n);

// Converte string para maiúsculas
void converteMaiscula(char texto[]);

//Libera a memória alocada para um array de 'Perguntas
void liberaRecursos(Pergunta *perguntas, int total_perguntas);

#endif
