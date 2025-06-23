#include "raylib.h"    // Biblioteca Raylib
#include <stdio.h>     // Para printf (debug no console)
#include <stdlib.h>    // Para exit, srand, rand
#include <time.h>      // Para time() para srand

// Inclua o cabeçalho das telas, que agora contém as definições de GameScreen e os protótipos das funções de tela.
#include "telas_jogo.h" // NOVO: Inclui o cabeçalho das telas modularizado

// Inclua os cabeçalhos das suas outras funcionalidades que o main.c chama diretamente no início.
#include "Headers/leitor_csv.h" // Para carregaPerguntasDeCSV
#include "Headers/funcoes_padrao.h" // Para liberaRecursos

// --- Variáveis Globais (Definições) ---
// Estas são as definições reais das variáveis globais que são declaradas como 'extern' em telas_jogo.h
Pergunta *g_perguntas = NULL;
int g_total_perguntas = 0;
int g_current_display_question_idx = 0; 
GameScreen currentScreen = GAME_LOGO; // Começa na tela de LOGO

// Variáveis de ESTADO DO JOGO PRINCIPAL (GAME_PLAYING_GRAPHICAL)
int g_current_level = 1;         
int g_correct_answers_in_row = 0;
int g_current_question_idx = -1;
GamePlayState g_game_play_state = PLAYING_QUESTION;
int g_answer_feedback_timer = 0;
bool g_is_answer_correct = false;
char g_selected_answer_char = ' ';
char g_correct_answer_char = ' ';
int framesCounter = 0;
Font g_marvel_font; // Variável global para a fonte personalizada


// --- FUNÇÃO main() ---
int main(void) {
    // Inicializa o gerador de números aleatórios uma única vez
    srand((unsigned)time(NULL)); 
    const int screenWidth = 1024;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Show do Milhao Marvel");
    
    // Ajusta a posição da janela
    int monitorWidth = GetMonitorWidth(0);
    int monitorHeight = GetMonitorHeight(0);
    int windowPosX = (monitorWidth - screenWidth) / 2;
    int windowPosY = (monitorHeight - screenHeight) / 2;
    SetWindowPosition(windowPosX, windowPosY + 30); // 30 pixels de offset para baixo
    SetTargetFPS(60);

    // Carrega a Fonte Personalizada
    g_marvel_font = LoadFont("assets/fonte/MarvelAlternativa.ttf"); 
    if (g_marvel_font.texture.id == 0) {
        printf("AVISO: Nao foi possivel carregar 'assets/fonte/MarvelAlternativa.ttf'. Usando fonte padrao.\n");
    }

    // Carrega perguntas do CSV
    printf("Carregando perguntas do arquivo CSV... (Verifique o console para feedback)\n");
    g_perguntas = carregaPerguntasDeCSV("questoes.csv", &g_total_perguntas);
    
    if (g_perguntas == NULL) {
        printf("AVISO: Nenhuma pergunta carregada ou erro ao abrir 'questoes.csv'.\n");
        g_total_perguntas = 0;
    } else {
        printf("Foram carregadas %d perguntas.\n", g_total_perguntas);
    }
    printf("Pressione ESC na janela Raylib para sair a qualquer momento.\n");

    // --- Loop Principal do Jogo (Raylib) ---
    while (!WindowShouldClose() && currentScreen != GAME_EXIT) { 
        // --- Lógica do Jogo ---
        switch (currentScreen) {
            case GAME_LOGO:             UpdateLogoScreen();             break;
            case GAME_TITLE:            UpdateTitleScreen();            break;
            case GAME_MAIN_MENU:        UpdateMainMenuScreen();         break;
            case GAME_DISPLAY_QUESTIONS: UpdateDisplayQuestionsScreen(); break;
            case GAME_PLAYING_GRAPHICAL: UpdatePlayingGraphicalScreen(); break;
            case GAME_INSERT_QUESTION:  UpdateInsertQuestionScreen();   break;
            case GAME_LIST_QUESTIONS:   UpdateListQuestionsScreen();    break;
            case GAME_SEARCH_QUESTION:  UpdateSearchQuestionScreen();   break;
            case GAME_ALTER_QUESTION:   UpdateAlterQuestionScreen();    break;
            case GAME_DELETE_QUESTION:  UpdateDeleteQuestionScreen();   break;
            case GAME_ENDING:           UpdateEndingScreen();           break;
            case GAME_PAUSE:                UpdatePauseScreen();        break;
            case GAME_WIN:              UpdateWinScreen();              break;
            case GAME_LOSE:             UpdateLoseScreen();             break;
            default: break;
        }

        // --- Desenho ---
        BeginDrawing();
            ClearBackground(MARVEL_DARKGRAY); // Fundo escuro Marvel

            switch (currentScreen) {
                case GAME_LOGO:             DrawLogoScreen();             break;
                case GAME_TITLE:            DrawTitleScreen();            break;
                case GAME_MAIN_MENU:        DrawMainMenuScreen();         break;
                case GAME_DISPLAY_QUESTIONS: DrawDisplayQuestionsScreen(); break;
                case GAME_PLAYING_GRAPHICAL: DrawPlayingGraphicalScreen(); break;
                case GAME_INSERT_QUESTION:  DrawInsertQuestionScreen();   break;
                case GAME_LIST_QUESTIONS:   DrawListQuestionsScreen();    break;
                case GAME_SEARCH_QUESTION:  DrawSearchQuestionScreen();   break;
                case GAME_ALTER_QUESTION:   DrawAlterQuestionScreen();    break;
            case GAME_DELETE_QUESTION:  DrawDeleteQuestionScreen();   break;
            case GAME_ENDING:           DrawEndingScreen();           break;
            case GAME_PAUSE:             DrawPauseScreen();        break;
            case GAME_WIN:              DrawWinScreen();              break;
            case GAME_LOSE:             DrawLoseScreen();             break;
            default: break;
        }

        EndDrawing();
    }

    // --- Desinicialização ---
    UnloadFont(g_marvel_font);
    if (g_perguntas != NULL) {
        liberaRecursos(g_perguntas, g_total_perguntas);
        g_perguntas = NULL;
    }
    CloseWindow();
    
    return 0;
}
