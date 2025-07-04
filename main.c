// main.c - O Coração do Jogo do Milhão Marvel (Modularizado com Todos os Sons ATIVOS)
// Contém a inicialização da Raylib, o loop principal e a chamada das telas.

#include "raylib.h"     // Biblioteca Raylib
#include <stdio.h>      // Para printf (debug no console)
#include <stdlib.h>     // Para exit, srand, rand
#include <string.h>     // Para funções de string
#include <time.h>       // Para time() para srand
#include <ctype.h>      // Para toupper

// Inclui o cabeçalho das telas, que agora contém as definições de GameScreen e os protótipos das funções de tela.
#include "telas_jogo.h" 

// Inclua os cabeçalhos das suas outras funcionalidades que o main.c chama diretamente no início.
#include "leitor_csv.h"     // Para carregaPerguntasDeCSV
#include "funcoes_padrao.h" // Para liberaRecursos (assumindo que liberaRecursos é de funcoes_padrao.h)
#include "ranking.h"        // Inclui o cabeçalho do ranking

// --- Variáveis Globais (Definições REAIS) ---
// Estas são as definições onde a memória é alocada e os valores iniciais são dados.
// Elas são declaradas como 'extern' em Headers/telas_jogo.h
Pergunta *g_perguntas = NULL;
int g_total_perguntas = 0;
// g_current_display_question_idx não é mais usada, pois GAME_DISPLAY_QUESTIONS foi removida
// int g_current_display_question_idx = 0; 
GameScreen currentScreen = GAME_LOGO; // Começa na tela de LOGO

// Variáveis de estado do jogo principal
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

// VARIÁVEIS GLOBAIS PARA SONS - Declaradas sem atribuição
Music g_music_background = {0}; 
Sound g_sound_correct = {0};     
Sound g_sound_wrong = {0};     
Sound g_sound_win = {0};     
Sound g_sound_lose = {0};      
Sound g_sound_menu_click = {0}; 
Sound g_sound_snap = {0};    

// NOVAS VARIÁVEIS PARA AS DICAS
int g_hint_exclude_used = 0;     
int g_hint_skip_used = 0;        
int g_hint_fifty_fifty_used = 0; 

bool g_fifty_fifty_active = false; 
char g_fifty_fifty_eliminated_chars[2] = {0, 0};      

// NOVAS VARIÁVEIS GLOBAIS PARA O RANKING
Ranking* g_ranking_board = NULL; // Ponteiro para o array de Rankings

// NOVAS VARIÁVEIS PARA A LÓGICA DA TELA DE FIM DE JOGO/RANKING
GameEndingState g_game_ending_state = ENDING_SHOW_SCORE; // Estado inicial da tela de fim de jogo
char g_player_name_input[MAX_LENGTH] = "\0"; // Buffer para o nome do jogador
int g_player_name_chars_count = 0;           // Contador de caracteres no buffer
int g_player_final_score = 0;                // Pontuação final do jogador para o ranking

// --- FUNÇÃO main() DO PROGRAMA ---
int main(void) {
    srand(time(NULL));

    const int screenWidth = 1024;
    const int screenHeight = 768;
    
    InitWindow(screenWidth, screenHeight, "Show do Milhao Marvel");
    
    int monitorWidth = GetMonitorWidth(0);
    int monitorHeight = GetMonitorHeight(0);
    int windowPosX = (monitorWidth - screenWidth) / 2;
    int windowPosY = (monitorHeight - screenHeight) / 2;
    SetWindowPosition(windowPosX, windowPosY + 30); // Posiciona a janela um pouco abaixo do centro

    SetTargetFPS(60);

    InitAudioDevice(); // Inicializa o dispositivo de áudio

    // Carrega a Fonte Personalizada
    g_marvel_font = LoadFont("assets/fonte/MarvelAlternativa.ttf"); 
    if (g_marvel_font.texture.id == 0) {
        printf("AVISO: Nao foi possivel carregar 'assets/fonte/MarvelAlternativa.ttf'. Usando fonte padrao.\n");
    }

    // CARREGA TODOS OS SONS E MÚSICAS
    g_music_background = LoadMusicStream("assets/sons/TemaMarvel.ogg"); 
    g_sound_correct = LoadSound("assets/sons/Acerto.ogg"); 
    g_sound_wrong = LoadSound("assets/sons/Erro.ogg");   
    g_sound_win = LoadSound("assets/sons/Vitoria.ogg");   
    g_sound_lose = LoadSound("assets/sons/Derrota.ogg");   
    g_sound_menu_click = LoadSound("assets/sons/Botao.ogg"); 
    g_sound_snap = LoadSound("assets/sons/estalo_thanos.ogg");

    // Carrega perguntas do CSV (chamada única no início)
    printf("Carregando perguntas do arquivo CSV... (Verifique o console para feedback)\n");
    g_perguntas = carregaPerguntasDeCSV("questoes.csv", &g_total_perguntas);
    
    if (g_perguntas == NULL || g_total_perguntas == 0) { // Verifica se g_total_perguntas é 0 também
        printf("AVISO: Nenhuma pergunta carregada ou erro ao abrir 'questoes.csv'.\n");
        g_total_perguntas = 0; // Garante que o contador seja 0 se a carga falhar
    } else {
        printf("Foram carregadas %d perguntas.\n", g_total_perguntas);
    }
    printf("Pressione ESC na janela Raylib para sair a qualquer momento.\n");

    // Carrega o Ranking ao iniciar o jogo
    g_ranking_board = lerRanking(); 
    if (g_ranking_board == NULL) {
        printf("ERRO CRITICO: Falha ao carregar ou inicializar o ranking. A funcionalidade de ranking pode estar desativada.\n");
    }

    // Inicia música de fundo
    PlayMusicStream(g_music_background);

    // --- Loop Principal do Jogo ---
    while (!WindowShouldClose() && currentScreen != GAME_EXIT) { 
        UpdateMusicStream(g_music_background); // Atualiza o stream da música de fundo

        // Atualiza tela conforme estado
        switch (currentScreen) {
            case GAME_LOGO:              UpdateLogoScreen();             break;
            case GAME_TITLE:             UpdateTitleScreen();            break;
            case GAME_MAIN_MENU:         UpdateMainMenuScreen();         break;
            case GAME_PLAYING_GRAPHICAL: UpdatePlayingGraphicalScreen(); break;
            // Telas de console - Suas Updates são minimalistas ou controladas pelo Edit Menu
            case GAME_INSERT_QUESTION:   /* UpdateInsertQuestionScreen(); */ break; 
            case GAME_LIST_QUESTIONS:    /* UpdateListQuestionsScreen(); */  break; 
            case GAME_SEARCH_QUESTION:   /* UpdateSearchQuestionScreen(); */ break;
            case GAME_ALTER_QUESTION:    /* UpdateAlterQuestionScreen(); */  break;
            case GAME_DELETE_QUESTION:   /* UpdateDeleteQuestionScreen(); */ break;
            case GAME_ENDING:            UpdateEndingScreen();           break;
            case GAME_WIN:               UpdateWinScreen();              break;
            case GAME_LOSE:              UpdateLoseScreen();             break;
            case GAME_RANKING:           UpdateRankingScreen();          break;
            case GAME_EDIT_MENU:         UpdateEditMenuScreen();         break;
            default: break;
        }

        // Renderização
        BeginDrawing();
            ClearBackground(MARVEL_DARKGRAY); // Limpa o fundo com a cor padrão

            // Desenha a tela atual conforme o estado
            switch (currentScreen) {
                case GAME_LOGO:              DrawLogoScreen();             break;
                case GAME_TITLE:             DrawTitleScreen();            break;
                case GAME_MAIN_MENU:         DrawMainMenuScreen();         break;
                // GAME_DISPLAY_QUESTIONS removido
                case GAME_PLAYING_GRAPHICAL: DrawPlayingGraphicalScreen(); break;
                // Telas de console - Apenas exibem mensagem estática
                case GAME_INSERT_QUESTION:   DrawInsertQuestionScreen();   break; 
                case GAME_LIST_QUESTIONS:    DrawListQuestionsScreen();    break;
                case GAME_SEARCH_QUESTION:   DrawSearchQuestionScreen();   break;
                case GAME_ALTER_QUESTION:    DrawAlterQuestionScreen();    break;
                case GAME_DELETE_QUESTION:   DrawDeleteQuestionScreen();   break;
                case GAME_ENDING:            DrawEndingScreen();           break;
                case GAME_WIN:               DrawWinScreen();              break;
                case GAME_LOSE:              DrawLoseScreen();             break;
                case GAME_RANKING:           DrawRankingScreen();          break;
                case GAME_EDIT_MENU:         DrawEditMenuScreen();         break;
                default: break;
            }

        EndDrawing();
    }

    // --- Desinicialização (Liberação de Recursos) ---
    UnloadFont(g_marvel_font); // Descarrega a fonte
    
    // Libera a memória das perguntas
    if (g_perguntas != NULL) {
        // A função liberaRecursos precisa liberar os textos das alternativas/enunciados se forem char*
        // Se forem char[], esta função só liberará o próprio array g_perguntas.
        // Já verificamos e ajustamos pergunta.c para char[] (strncpy + free temp).
        liberaRecursos(g_perguntas, g_total_perguntas); 
        g_perguntas = NULL;
    }
    
    // Libera a memória do ranking
    if (g_ranking_board != NULL) {
        free(g_ranking_board); // O ranking é um array de Ranking*, não precisa de loop para free se não contiver ponteiros internos
        g_ranking_board = NULL;
    }
    
    // Descarrega assets de áudio
    UnloadMusicStream(g_music_background);
    UnloadSound(g_sound_correct);
    UnloadSound(g_sound_wrong);
    UnloadSound(g_sound_win);
    UnloadSound(g_sound_lose);
    UnloadSound(g_sound_menu_click);
    UnloadSound(g_sound_snap);
     
    
    CloseAudioDevice(); // Fecha o dispositivo de áudio
    CloseWindow();      // Fecha a janela Raylib

    return 0;
}