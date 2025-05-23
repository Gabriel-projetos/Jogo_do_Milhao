#ifndef PERGUNTA_H
#define PERGUNTA_H

#include "alternativa.h"
#include "dificuldade.h"

typedef struct{
     char *enunciado;
     Alternativa alternativas[4];
     char correta[2];
     Dificuldade dificuldade;
}Pergunta;

void recebePergunta(Pergunta *perguntas, int indice);
void mostraPergunta(Pergunta perguntas);
void listaPerguntas(Pergunta *perguntas, int total_perguntas);
void pesquisaPergunta(Pergunta *perguntas);
void alterarPergunta(Pergunta *perguntas);
void excluiPergunta(Pergunta *perguntas, int total_perguntas);

#endif