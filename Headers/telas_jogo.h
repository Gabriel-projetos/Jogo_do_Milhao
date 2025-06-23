#ifndef TELAS_JOGO_H
#define TELAS_JOGO_H

#include "raylib.h" 
#include <stdio.h>  
#include <ctype.h>  
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

// --- Estados Internos da Tela de Jogo ---
typedef enum GamePlayState {
    PLAYING_QUESTION = 0,
    PLAYING_FEEDBACK,
    PLAYING_NEXT_QUESTION
} GamePlayState;

// --- Variáveis Globais ---
extern Pergunta *g_perguntas;
extern int g_total_perguntas;
extern int g_current_display_question_idx;
extern GameScreen currentScreen; // O estado global da tela atual

// Variáveis de ESTADO DO JOGO PRINCIPAL
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

// --- Definições de Cores Personalizadas para o Tema Marvel ---
#define MARVEL_RED       (Color){ 178, 20, 30, 255 }
#define MARVEL_BLUE      (Color){ 10, 80, 150, 255 }
#define MARVEL_GOLD      (Color){ 255, 215, 0, 255 }
#define MARVEL_GRAY      (Color){ 50, 50, 50, 255 }
#define MARVEL_LIGHTGRAY (Color){ 150, 150, 150, 255 }
#define MARVEL_DARKGRAY  (Color){ 30, 30, 30, 255 }
#define COLOR_CORRETO    (Color){ 0, 150, 0, 255 }
#define COLOR_ERRADO     (Color){ 150, 0, 0, 255 }

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

// Funções auxiliares
bool GuiButton(Rectangle bounds, const char *text, Color buttonColor, Color textColor);
void ResetGamePlayingState(void);
void LoadNextQuestion(void);

#endif // TELAS_JOGO_H
