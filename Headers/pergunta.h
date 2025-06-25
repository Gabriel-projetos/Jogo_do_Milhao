#ifndef PERGUNTA_H
#define PERGUNTA_H

#include <stdio.h>    
#include <stdlib.h>   
#include <string.h>   
#include <ctype.h>    

#include "alternativa.h"
#include "dificuldade.h"

typedef struct {
    char *enunciado;
    Alternativa alternativas[4];
    char correta;
    Dificuldade nivel;
    int ja_foi_usada; // ← NOVO: 0 = não usada, 1 = já usada
} Pergunta;


// Protótipos das funções para manipulação das perguntas

// Recebe uma nova pergunta, adiciona ao vetor de perguntas e atualiza total
void recebePergunta(Pergunta **perguntas, int *total);

// Lista todas as perguntas, organizadas por nível
void listaPerguntas(Pergunta *perguntas, int total); // Corrigi o tipo do segundo parâmetro para int*

// Pesquisa perguntas que contenham um texto no enunciado e exibe resultados
void pesquisaPergunta(Pergunta *perguntas, int total);

// Altera pergunta em um índice especificado
void alterarPergunta(Pergunta *perguntas, int total);

// Exclui pergunta em índice especificado e realoca vetor
void excluirPergunta(Pergunta **perguntas, int *total);

void mostraPergunta(Pergunta *pergunta);

void salvaPerguntaNoCSV(const char *questoes, Pergunta *pergunta);
#endif
