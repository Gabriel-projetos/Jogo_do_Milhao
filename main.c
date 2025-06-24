#include "raylib.h"    // Biblioteca Raylib
#include <stdio.h>     // Para printf (debug no console)
#include <stdlib.h>    // Para exit, srand, rand
#include <time.h>      // Para time() para srand
#include "telas_jogo.h" // Inclui o cabeçalho das telas modularizado
#include "Headers/leitor_csv.h"     // Para carregaPerguntasDeCSV
#include "Headers/funcoes_padrao.h" // Para liberaRecursos

// --- Variáveis Globais (Definições) ---
Pergunta *g_perguntas = NULL;
int g_total_perguntas = 0;
int g_current_display_question_idx = 0; 
GameScreen currentScreen = GAME_LOGO; // Começa na tela de LOGO

// --- Variáveis de ESTADO DO JOGO PRINCIPAL --- 
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

// --- VARIÁVEIS GLOBAIS PARA SONS --- (Definições - mantenham comentadas enquanto não tem os arquivos .mp3/.wav carregados)
//Music g_music_background = {0}; // Inicialize com {0}
//Sound g_sound_correct = {0};    // Inicialize com {0}
//Sound g_sound_wrong = {0};      // Inicialize com {0}
//Sound g_sound_win = {0};        // Inicialize com {0}
//Sound g_sound_lose = {0};       // Inicialize com {0}
//Sound g_sound_menu_click = {0}; // Inicialize com {0}

// --- NOVAS VARIÁVEIS PARA AS DICAS ---
int g_hint_exclude_used = 0;     // Contagem de uso da dica "Excluir Questão" (max 1 por partida)
int g_hint_skip_used = 0;        // Contagem de uso da dica "Pular Questão" (max 2 por partida)
int g_hint_fifty_fifty_used = 0; // Contagem de uso da dica "Meio a Meio" (max 3 por partida)
bool g_fifty_fifty_active = false; // Flag para saber se a dica Meio a Meio está ativa para a pergunta atual
char g_fifty_fifty_eliminated_chars[2] = {0, 0}; // Guarda as letras das alternativas eliminadas pela dica 50/50

// --- FUNÇÃO main() DO PROGRAMA ---
int main(void) {
    srand((unsigned)time(NULL)); 

    const int screenWidth = 1024;
    const int screenHeight = 768;
    
    InitWindow(screenWidth, screenHeight, "Show do Milhao Marvel");
    
    int monitorWidth = GetMonitorWidth(0);
    int monitorHeight = GetMonitorHeight(0);
    int windowPosX = (monitorWidth - screenWidth) / 2;
    int windowPosY = (monitorHeight - screenHeight) / 2;
    SetWindowPosition(windowPosX, windowPosY + 30); // 30 pixels de offset para baixo

    SetTargetFPS(60);

    // INICIALIZA O DISPOSITIVO DE ÁUDIO (Mantenha, mesmo se os sons estiverem comentados)
    InitAudioDevice();

    // Carrega a Fonte Personalizada
    g_marvel_font = LoadFont("assets/fonte/MarvelAlternativa.ttf"); 
    if (g_marvel_font.texture.id == 0) {
        printf("AVISO: Nao foi possivel carregar 'assets/fonte/MarvelAlternativa.ttf'. Usando fonte padrao.\n");
    }

    // --- CARREGA SONS E MÚSICAS --- (Mantenham comentadas enquanto não tem os arquivos)
    // g_music_background = LoadMusicStream("resources/sounds/tema_marvel.mp3");
    // g_sound_correct = LoadSound("resources/sounds/acerto.wav");
    // g_sound_wrong = LoadSound("resources/sounds/erro.wav");
    // g_sound_win = LoadSound("resources/sounds/vitoria.wav");
    // g_sound_lose = LoadSound("resources/sounds/derrota.wav");
    // g_sound_menu_click = LoadSound("resources/sounds/menu_click.wav");
    // // REPRODUZ MÚSICA DE FUNDO NO INÍCIO DO JOGO (Mantenham comentada)
    // PlayMusicStream(g_music_background);

    // --- Carrega perguntas do CSV (chamada única no início) ---
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
        // ATUALIZA O FLUXO DA MÚSICA DE FUNDO (Mantenha comentada)
        // UpdateMusicStream(g_music_background);

        // --- Atualização da Lógica do Jogo (Chamando a função de Update da tela atual) ---
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
            case GAME_PAUSE:            // UpdatePauseScreen();        break;
            case GAME_WIN:              UpdateWinScreen();              break;
            case GAME_LOSE:             UpdateLoseScreen();             break;
            default: break;
        }

        // --- Desenho (Renderização de tudo na tela, chamando a função de Draw da tela atual) ---
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
                case GAME_PAUSE:            // DrawPauseScreen();        break;
                case GAME_WIN:              DrawWinScreen();              break;
                case GAME_LOSE:             DrawLoseScreen();             break;
                default: break;
            }

        EndDrawing();
    }

    // --- Desinicialização ---
    // DESCARREGA TODOS OS RECURSOS DE ÁUDIO (Mantenham comentados enquanto não tem sons)
    // UnloadMusicStream(g_music_background);
    // UnloadSound(g_sound_correct);
    // UnloadSound(g_sound_wrong);
    // UnloadSound(g_sound_win);
    // UnloadSound(g_sound_lose);
    // UnloadSound(g_sound_menu_click);
    CloseAudioDevice(); // FECHA O DISPOSITIVO DE ÁUDIO (Importante chamar, mesmo sem sons carregados)

    UnloadFont(g_marvel_font);
    if (g_perguntas != NULL) {
        liberaRecursos(g_perguntas, g_total_perguntas);
        g_perguntas = NULL;
    }
    CloseWindow(); // Fecha a janela da Raylib

    return 0;
}

