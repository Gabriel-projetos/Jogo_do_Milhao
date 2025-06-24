#ifndef TELAS_JOGO_H
#define TELAS_JOGO_H

#include "raylib.h" 
#include <stdio.h>  
#include <ctype.h>  
#include <stdbool.h> 
#include "Headers/pergunta.h" 
#include "Headers/leitor_csv.h" 
#include "Headers/funcoes_padrao.h" 
#include "Headers/menu_inicial.h"   
#include "Headers/jogo.h"           


// --- Definição de Estados do Jogo ---
typedef enum GameScreen {
    GAME_LOGO = 0,
    GAME_TITLE,
    GAME_MAIN_MENU,         // Menu principal com botões gráficos
    GAME_DISPLAY_QUESTIONS, // Estado para exibir perguntas uma a uma
    GAME_PLAYING_GRAPHICAL, // ESTADO DO JOGO INTERATIVO
    GAME_INSERT_QUESTION,   // Interação via console
    GAME_LIST_QUESTIONS,    // Interação via console
    GAME_SEARCH_QUESTION,   // Interação via console
    GAME_ALTER_QUESTION,    // Interação via console
    GAME_DELETE_QUESTION,   // Interação via console
    GAME_ENDING,            // Tela genérica de fim de jogo
    GAME_PAUSE,
    GAME_WIN,               // Tela de vitória específica
    GAME_LOSE,
    GAME_EXIT = -1          
} GameScreen;

// --- Estados Internos da Tela de Jogo (GAME_PLAYING_GRAPHICAL) ---
typedef enum GamePlayState {
    PLAYING_QUESTION = 0,    
    PLAYING_FEEDBACK,     
    PLAYING_NEXT_QUESTION  
} GamePlayState;

// --- Variáveis Globais ---
extern Pergunta *g_perguntas;
extern int g_total_perguntas;
extern int g_current_display_question_idx;
extern GameScreen currentScreen;

// --- Variáveis de ESTADO DO JOGO PRINCIPAL ---
extern int g_current_level;         
extern int g_correct_answers_in_row;
extern int g_current_question_idx;
extern GamePlayState g_game_play_state;
extern int g_answer_feedback_timer;
extern bool g_is_answer_correct;
extern char g_selected_answer_char;
extern char g_correct_answer_char;
extern int framesCounter; // Contador de frames para as telas de logo/introdução
extern Font g_marvel_font; // A fonte personalizada

// --- VARIÁVEIS GLOBAIS PARA SONS (Sons, já estão declarados aqui) ---
extern Music g_music_background;
extern Sound g_sound_correct;
extern Sound g_sound_wrong;
extern Sound g_sound_win;
extern Sound g_sound_lose;
extern Sound g_sound_menu_click;

// --- NOVAS VARIÁVEIS PARA AS DICAS ---
extern int g_hint_exclude_used; // Contador para a dica "Excluir Questão" (max 1 uso por partida)
extern int g_hint_skip_used;    // Contador para a dica "Pular Questão" (max 2 usos por partida)
extern int g_hint_fifty_fifty_used; // Contador para a dica "Meio a Meio" (max 3 usos por partida)

extern bool g_fifty_fifty_active; // Flag para saber se a dica Meio a Meio está ativa para a pergunta atual
extern char g_fifty_fifty_eliminated_chars[2]; // Guarda as letras das alternativas eliminadas pela dica 50/50

// --- Definições de Cores Personalizadas para o Tema Marvel ---
#define MARVEL_RED       (Color){ 178, 20, 30, 255 }
#define MARVEL_BLUE      (Color){ 10, 80, 150, 255 }
#define MARVEL_GOLD      (Color){ 255, 215, 0, 255 }
#define MARVEL_GRAY      (Color){ 50, 50, 50, 255 }
#define MARVEL_LIGHTGRAY (Color){ 150, 150, 150, 255 }
#define MARVEL_DARKGRAY  (Color){ 30, 30, 30, 255 }
#define COLOR_CORRETO    (Color){ 0, 150, 0, 255 }
#define COLOR_ERRADO     (Color){ 150, 0, 0, 255 }
#define COLOR_HINT_AVAILABLE (Color){ 100, 0, 150, 255 } // Cor para botão de dica disponível
#define COLOR_HINT_USED     (Color){ 60, 0, 90, 255 }  // Cor para botão de dica já usado

// --- Protótipos das Funções das Telas (Update e Draw) ---
void UpdateLogoScreen(void);
void UpdateTitleScreen(void);
void UpdateMainMenuScreen(void);
void UpdateDisplayQuestionsScreen(void);
void UpdatePlayingGraphicalScreen(void);
void UpdateInsertQuestionScreen(void);
void UpdateListQuestionsScreen(void);
void UpdateSearchQuestionScreen(void);
void UpdateAlterQuestionScreen(void);
void UpdateDeleteQuestionScreen(void);
void UpdateEndingScreen(void);
void UpdateWinScreen(void);
void UpdateLoseScreen(void);
void DrawLogoScreen(void);
void DrawTitleScreen(void);
void DrawMainMenuScreen(void);
void DrawDisplayQuestionsScreen(void);
void DrawPlayingGraphicalScreen(void);
void DrawInsertQuestionScreen(void);
void DrawListQuestionsScreen(void);
void DrawSearchQuestionScreen(void);
void DrawAlterQuestionScreen(void);
void DrawDeleteQuestionScreen(void);
void DrawEndingScreen(void);
void DrawWinScreen(void);
void DrawLoseScreen(void);

// --- Funções auxiliares (UI e Jogo) ---
bool GuiButton(Rectangle bounds, const char *text, Color buttonColor, Color textColor);
void ResetGamePlayingState(void); // Reseta variáveis de jogo (incluindo dicas)
void LoadNextQuestion(void);      // Carrega a próxima pergunta sorteada

#endif // TELAS_JOGO_H
