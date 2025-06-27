#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h>
#include "pergunta.h"
#include "jogo.h"
#include "funcoes_padrao.h"

// Nota: A inclusão de "raylib.h" pode ser necessária se GetRandomValue() for usado aqui.
// Como você está usando rand(), as inclusões atuais são suficientes.

/**
 * @brief Sorteia Perguntas por Nível de Dificuldade, evitando repetidas.
 * @param perguntas Array de Pergunta
 * @param total Número total de perguntas
 * @param nivel Nível de dificuldade (1 a 5)
 * @return Ponteiro para a Pergunta sorteada ou NULL se não encontrar
 */
Pergunta* sorteiaPorNivel(Pergunta *perguntas, int total, int nivel) {
    int indices_validos[total];
    int contador = 0;

    // Coleta os índices de perguntas disponíveis do nível desejado
    for (int i = 0; i < total; i++) {
        // CORREÇÃO: Cast para int para evitar o aviso de signed/unsigned comparison
        // Garante que a comparação entre 'perguntas[i].nivel' (que é Dificuldade/enum)
        // e 'nivel' (que é int) seja segura.
        if ((int)perguntas[i].nivel == nivel && perguntas[i].ja_foi_usada == 0) { 
            indices_validos[contador++] = i;
        }
    }

    if (contador == 0) {
        return NULL; // Nenhuma pergunta disponível para este nível
    }

    // Sorteia um índice aleatório entre os válidos
    // `rand() % contador` é uma forma simples, mas `GetRandomValue(0, contador - 1)` da Raylib
    // é geralmente preferível para aplicações de jogos por ser mais robusto.
    int sorteado_idx = rand() % contador; 
    
    // Marca a pergunta como usada para não ser sorteada novamente
    perguntas[indices_validos[sorteado_idx]].ja_foi_usada = 1;
    
    // Retorna o ponteiro para a pergunta sorteada
    return &perguntas[indices_validos[sorteado_idx]];
}


/**
 * @brief Função que salva o progresso do jogador em um arquivo binário
 * @param progresso Ponteiro para a estrutura ProgressoJogador que contém os dados do jogador
 * @param nome_arquivo Nome do arquivo onde o progresso será salvo
 */
void salvaProgresso(ProgressoJogador *progresso, const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "ab"); // Abre para acrescentar no final (append binary)
    if (!fp) {
        printf("Erro ao abrir arquivo para salvar progresso: %s\n", nome_arquivo); // Mensagem de erro mais descritiva
        return;
    }
    fwrite(progresso, sizeof(ProgressoJogador), 1, fp);
    fclose(fp);
}

/**
 * @brief Loop principal do jogo que faz a lógica de perguntas e níveis (versão de console).
 * Esta função parece ser uma versão console do jogo, usada paralelamente à GUI.
 * @param perguntas Array de Pergunta
 * @param total Número total de perguntas
 */
void jogoAcontece(Pergunta perguntas[], int total) {
    int pergunta_atual = 1;
    char resposta;
    int acertos = 0;

    // Loop principal do jogo (baseado em console)
    while (pergunta_atual <= 15) { // Joga até 15 perguntas
        int nivel;

        // Define o nível da pergunta com base na pergunta atual do jogo
        if (pergunta_atual <= 2)
            nivel = 1; // Muito fácil
        else if (pergunta_atual <= 4)
            nivel = 2; // Fácil
        else if (pergunta_atual <= 8)
            nivel = 3; // Médio
        else if (pergunta_atual <= 12)
            nivel = 4; // Difícil
        else
            nivel = 5; // Muito difícil

        Pergunta *pergunta_sorteada = sorteiaPorNivel(perguntas, total, nivel);
        if (pergunta_sorteada == NULL) {
            printf("Não há mais perguntas disponíveis para o nível %d.\n", nivel);
            break; // Sai do jogo se não houver mais perguntas
        }

        mostraPergunta(pergunta_sorteada); // Mostra a pergunta no console
        printf("Digite a letra da alternativa correta: ");
        limpaBuffer(); // Limpa o buffer de entrada
        scanf(" %c", &resposta);
        resposta = toupper(resposta); // Converte a resposta para maiúscula

        if (resposta == pergunta_sorteada->correta) {
            printf("\033[0;32mCorreto!\033[0m\n"); // Mensagem de acerto (com cor)
            acertos++;
            pergunta_atual++;
        } else {
            printf("\033[0;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_sorteada->correta); // Mensagem de erro (com cor)

            // Salva o progresso do jogador ao errar
            ProgressoJogador progresso;
            printf("Digite seu nome para salvar o progresso: ");
            scanf("%49s", progresso.nome); // Lê o nome (limite de 49 caracteres + null terminator)
            progresso.acertos = acertos;
            salvaProgresso(&progresso, "progresso.bin"); // Salva no arquivo binário
            return; // Encerra o jogo no console
        }
    }

    // Se o jogador passar por todas as 15 perguntas
    if (pergunta_atual > 15) {
        printf("\033[1;32mParabéns! Você venceu o jogo!\033[0m\n");
        // Salva o progresso final do jogador
        ProgressoJogador progresso;
        printf("Digite seu nome para salvar o progresso: ");
        scanf("%49s", progresso.nome);
        progresso.acertos = acertos;
        salvaProgresso(&progresso, "progresso.bin");
    }
}

/**
 * @brief Exibe a pergunta do milhão e trata resposta final (versão de console).
 * Esta função parece ser para um modo de "pergunta do milhão" específico, talvez não integrado
 * diretamente com o fluxo principal de 'jogoAcontece' que já vai até o nível 5.
 * @param pergunta_do_milhao Ponteiro para a pergunta do milhão
 */
void perguntaDoMilhao(Pergunta *pergunta_do_milhao) {
    char resposta;
    printf("\033[1;33mParabéns! Você chegou à pergunta do milhão!\033[0m\n");
    mostraPergunta(pergunta_do_milhao); // Mostra a pergunta no console
    printf("Digite a letra da alternativa correta: ");
    limpaBuffer(); // Limpa o buffer de entrada
    scanf(" %c", &resposta);
    resposta = toupper(resposta); // Converte a resposta para maiúscula

    if (resposta == pergunta_do_milhao->correta) {
        printf("\033[1;32mParabéns! Você ganhou o jogo!\033[0m\n");
    } else {
        printf("\033[1;31mErrado! A resposta correta era %c.\033[0m\n", pergunta_do_milhao->correta);
    }
}

/**
 * @brief Libera memória alocada para perguntas e alternativas.
 * Importante para evitar vazamentos de memória ao finalizar o programa ou recarregar perguntas.
 * @param perguntas Array de Pergunta
 * @param total_perguntas Número total de perguntas para liberar
 */
void liberaRecursos(Pergunta *perguntas, int total_perguntas) {
    for (int i = 0; i < total_perguntas; i++) {
        // Verifica se enunciado e texto foram alocados dinamicamente (char*).
        // Se no pergunta.h eles são char[] (arrays fixos), estas chamadas `free` são incorretas.
        // Assumindo que `leTextoDinamico` aloca e esses campos são `char*` na struct,
        // mas as correções recentes assumiram `char[]` para `pergunta.c`.
        // SE FOREM CHAR[] FIXOS NO PERGUNTA.H, REMOVA OS FREES DOS TEXTOS.
        if (perguntas[i].enunciado != NULL) {
            // free(perguntas[i].enunciado); // Remova se enunciado for char[]
            // perguntas[i].enunciado = NULL;
        }
        for (int j = 0; j < 4; j++) {
            if (perguntas[i].alternativas[j].texto != NULL) {
                // free(perguntas[i].alternativas[j].texto); // Remova se texto for char[]
                // perguntas[i].alternativas[j].texto = NULL;
            }
        }
    }
    // Libera o array principal de perguntas
    if (perguntas != NULL) {
        free(perguntas);
        perguntas = NULL;
    }
}