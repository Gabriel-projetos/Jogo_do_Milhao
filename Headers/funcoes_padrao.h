#ifndef FUNCOES_PADRAO_H
#define FUNCOES_PADRAO_H

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

#endif
