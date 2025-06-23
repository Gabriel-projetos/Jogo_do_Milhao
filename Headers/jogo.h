#ifndef JOGO_H
#define JOGO_H

#include "pergunta.h"

Pergunta* sorteiaPergunta(Pergunta *perguntas, int total);
void embaralhaPerguntas(Pergunta **originais, Pergunta **embaralhadas, int total);
Pergunta* sorteiaPorNivel(Pergunta *perguntas, int total, int nivel);
void jogoAcontece(Pergunta *perguntas, int total); 
void perguntaDoMilhao(Pergunta *perguntas); 

#endif // JOGO_H
