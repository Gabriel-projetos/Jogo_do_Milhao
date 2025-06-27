// Headers/telas_jogo.h - Definições Centrais para o Jogo Raylib

#ifndef TELAS_JOGO_H
#define TELAS_JOGO_H

#include "raylib.h"    // Inclui Raylib para tipos como Rectangle, Color, Vector2, Font, etc.
#include <stdio.h>     // Para sprintf (usado em TextFormat)
#include <ctype.h>     // Para toupper (usado em lógica de botões e respostas)
#include <stdbool.h>   // Para tipo bool

#include "pergunta.h"
#include "leitor_csv.h"
#include "funcoes_padrao.h"
#include "menu_inicial.h"
#include "jogo.h"          
#include "ranking.h"               // Inclui o cabeçalho do ranking para a struct Ranking e LEADER_SIZE

// --- Definição de Estados do Jogo ---
typedef enum GameScreen {
    GAME_LOGO = 0,
    GAME_TITLE,
    GAME_MAIN_MENU,          // Menu principal com botões gráficos
    GAME_DISPLAY_QUESTIONS,  // Estado para exibir perguntas uma a uma
    GAME_PLAYING_GRAPHICAL,  // ESTADO DO JOGO INTERATIVO
    GAME_INSERT_QUESTION,    // Interação via console
    GAME_LIST_QUESTIONS,     // Interação via console
    GAME_SEARCH_QUESTION,    // Interação via console
    GAME_ALTER_QUESTION,     // Interação via console
    GAME_DELETE_QUESTION,    // Interação via console
    GAME_ENDING,             // Tela genérica de fim de jogo (agora com lógica de input de nome e visualização)
    GAME_PAUSE,
    GAME_WIN,                // Tela de vitória específica
    GAME_LOSE,               // Tela de derrota específica
    GAME_RANKING,            // NOVO: Tela dedicada para exibir o ranking a qualquer momento
    GAME_EDIT_MENU,          // NOVO: Tela de edição de perguntas (opção no menu principal)
    GAME_EXIT = -1           // Valor para indicar que o jogo deve sair
} GameScreen;

// --- Estados Internos da Tela de Jogo (GAME_PLAYING_GRAPHICAL) ---
typedef enum GamePlayState {
    PLAYING_QUESTION = 0,    // Mostrando a pergunta, esperando resposta do jogador
    SHOWING_FEEDBACK,        // NOVO NOME: Era PLAYING_FEEDBACK, agora ajustado para SHOWING_FEEDBACK para consistência
    PLAYING_NEXT_QUESTION,
    LEVEL_COMPLETE           // NOVO ESTADO: Para transição suave após resposta correta para próxima pergunta
} GamePlayState;


// --- NOVOS Estados Internos da Tela de Fim de Jogo (GAME_ENDING) ---
// Usado para gerenciar o fluxo de entrada de nome e exibição do ranking na tela GAME_ENDING.
typedef enum {
    ENDING_SHOW_SCORE,   // Estado inicial: mostra a pontuação final
    ENDING_INPUT_NAME,   // Estado: aguarda a entrada do nome do jogador para o ranking
    ENDING_SHOW_RANKING  // Estado: exibe a lista do ranking
} GameEndingState;


// --- Variáveis Globais (declaradas como 'extern' aqui, DEFINIDAS em main.c) ---
extern Pergunta *g_perguntas;
extern int g_total_perguntas;
extern int g_current_display_question_idx;
extern GameScreen currentScreen; // O estado global da tela atual

// Variáveis de ESTADO DO JOGO PRINCIPAL (GAME_PLAYING_GRAPHICAL)
extern int g_current_level;          
extern int g_correct_answers_in_row;
extern int g_current_question_idx;
extern GamePlayState g_game_play_state; // Renomeado no enum acima
extern int g_answer_feedback_timer;
extern bool g_is_answer_correct;
extern char g_selected_answer_char;
extern char g_correct_answer_char; 
extern int framesCounter; // Contador de frames para as telas de logo/introdução
extern Font g_marvel_font; // A fonte personalizada

// VARIÁVEIS GLOBAIS PARA SONS
extern Music g_music_background;    // Música de fundo
extern Sound g_sound_correct;       // Som de resposta correta
extern Sound g_sound_wrong;         // Som de resposta errada
extern Sound g_sound_win;           // Som de vitória
extern Sound g_sound_lose;          // Som de derrota
extern Sound g_sound_menu_click;    // Som de clique no menu
extern Sound g_sound_snap;          // Som do estalo dos dedos

// VARIÁVEIS PARA AS DICAS
extern int g_hint_exclude_used; 
extern int g_hint_skip_used;    
extern int g_hint_fifty_fifty_used; 
extern bool g_fifty_fifty_active; 
extern char g_fifty_fifty_eliminated_chars[2]; 

// NOVAS VARIÁVEIS GLOBAIS PARA O RANKING (extern)
extern Ranking* g_ranking_board; // Ponteiro para o array de Rankings

// NOVAS VARIÁVEIS PARA A LÓGICA DA TELA DE FIM DE JOGO/RANKING (extern)
extern GameEndingState g_game_ending_state; // Estado da tela de fim de jogo (mostra score, input nome, mostra ranking)
extern char g_player_name_input[MAX_LENGTH]; // Buffer para a entrada de nome do jogador
extern int g_player_name_chars_count;       // Contador de caracteres no buffer de nome
extern int g_player_final_score;            // A pontuação final do jogador (para ser usada no ranking)


// --- Definições de Cores Personalizadas para o Tema Marvel ---
#define MARVEL_RED              (Color){ 178, 20, 30, 255 }
#define MARVEL_BLUE             (Color){ 10, 80, 150, 255 }
#define MARVEL_GOLD             (Color){ 255, 215, 0, 255 }
#define MARVEL_GRAY             (Color){ 50, 50, 50, 255 }
#define MARVEL_LIGHTGRAY        (Color){ 150, 150, 150, 255 }
#define MARVEL_DARKGRAY         (Color){ 30, 30, 30, 255 }
#define COLOR_CORRETO           (Color){ 0, 150, 0, 255 }
#define COLOR_ERRADO            (Color){ 150, 0, 0, 255 } 
#define COLOR_HINT_AVAILABLE_PURPLE (Color){ 100, 0, 150, 255 } 
#define COLOR_HINT_USED_PURPLE      (Color){ 60, 0, 90, 255 }   


// --- Protótipos das Funções de Lógica (Update) e Desenho (Draw) para CADA TELA ---
void SetGameScreen(GameScreen screen);

void UpdateLogoScreen(void);
void DrawLogoScreen(void);

void UpdateTitleScreen(void);
void DrawTitleScreen(void);

void UpdateMainMenuScreen(void);
void DrawMainMenuScreen(void);

void UpdateDisplayQuestionsScreen(void);
void DrawDisplayQuestionsScreen(void);

void UpdatePlayingGraphicalScreen(void);
void DrawPlayingGraphicalScreen(void);

void UpdateInsertQuestionScreen(void);
void DrawInsertQuestionScreen(void);

void UpdateListQuestionsScreen(void); 
void DrawListQuestionsScreen(void);

void UpdateSearchQuestionScreen(void);
void DrawSearchQuestionScreen(void);

void UpdateAlterQuestionScreen(void);
void DrawAlterQuestionScreen(void);

void UpdateDeleteQuestionScreen(void);
void DrawDeleteQuestionScreen(void);

void UpdateEndingScreen(void); // Gerencia múltiplos estados (score, input, ranking)
void DrawEndingScreen(void);   // Desenha múltiplos estados

void UpdateWinScreen(void);
void DrawWinScreen(void);

void UpdateLoseScreen(void);
void DrawLoseScreen(void);

//Protótipos para a tela de ranking dedicada
void UpdateRankingScreen(void);
void DrawRankingScreen(void);

void UpdateEditMenuScreen(void);
void DrawEditMenuScreen(void);

// Funções auxiliares (UI e Jogo) - seus protótipos também ficam aqui
bool GuiButton(Rectangle bounds, const char *text, Color buttonColor, Color textColor);
void ResetGamePlayingState(void);
void LoadNextQuestion(void);

#endif // TELAS_JOGO_H
