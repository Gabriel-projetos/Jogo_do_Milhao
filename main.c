// main.c - O Coração do Jogo do Milhão Marvel (Modularizado com Todos os Sons ATIVOS)
// Contém a inicialização da Raylib, o loop principal e a chamada das telas.

#include "raylib.h"    // Biblioteca Raylib
#include <stdio.h>     // Para printf (debug no console)
#include <stdlib.h>    // Para exit, srand, rand
#include <string.h>    // Para funções de string
#include <time.h>      // Para time() para srand
#include <ctype.h>     // Para toupper

// Inclui o cabeçalho das telas, que agora contém as definições de GameScreen e os protótipos das funções de tela.
#include "Headers/telas_jogo.h" 

// Inclua os cabeçalhos das suas outras funcionalidades que o main.c chama diretamente no início.
#include "Headers/leitor_csv.h"     // Para carregaPerguntasDeCSV
#include "Headers/funcoes_padrao.h" // Para liberaRecursos

// --- Variáveis Globais (Definições REAIS) ---
// Estas são as definições onde a memória é alocada e os valores iniciais são dados.
// Elas são declaradas como 'extern' em Headers/telas_jogo.h
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

// VARIÁVEIS GLOBAIS PARA SONS (Definições e Inicialização AGORA ATIVAS)
// CERTIFIQUE-SE DE QUE ESTES ARQUIVOS EXISTEM NOS CAMINHOS ESPECIFICADOS EM SEU PROJETO!
Music g_music_background = {0}; 
Sound g_sound_correct = {0};    
Sound g_sound_wrong = {0};      
Sound g_sound_win = {0};        
Sound g_sound_lose = {0};       
Sound g_sound_menu_click = {0}; 

// NOVAS VARIÁVEIS PARA AS DICAS (Definições e Inicialização)
int g_hint_exclude_used = 0;     
int g_hint_skip_used = 0;        
int g_hint_fifty_fifty_used = 0; 

bool g_fifty_fifty_active = false; 
char g_fifty_fifty_eliminated_chars[2] = {0, 0}; 

// VARIÁVEIS GLOBAIS PARA A MANOPLA DO ESTALO (Definições)
Texture2D g_texture_gauntlet; 
Sound g_sound_snap;           
bool g_gauntlet_snap_active = false; 
int g_gauntlet_snap_timer = 0;       


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
    SetWindowPosition(windowPosX, windowPosY + 30);

    SetTargetFPS(60);

    InitAudioDevice(); // Inicializa o dispositivo de áudio

    // Carrega a Fonte Personalizada
    g_marvel_font = LoadFont("assets/fonte/MarvelAlternativa.ttf"); 
    if (g_marvel_font.texture.id == 0) {
        printf("AVISO: Nao foi possivel carregar 'assets/fonte/MarvelAlternativa.ttf'. Usando fonte padrao.\n");
    }

    // CARREGA TODOS OS SONS E MÚSICAS (Descomentados e usando seus caminhos)
    g_music_background = LoadMusicStream("assets/sounds/TemaMarvel.mp3"); 
    g_sound_correct = LoadSound("assets/sounds/Acerto.mp3"); 
    g_sound_wrong = LoadSound("assets/sounds/Erro.mp3");   
    g_sound_win = LoadSound("assets/sounds/Vitoria.mp3");  
    g_sound_lose = LoadSound("assets/sounds/Derrota.mp3");  
    g_sound_menu_click = LoadSound("assets/sounds/Botao.mp3"); 

    // CARREGA ATIVOS DA MANOPLA (Imagem e Som do Estalo)
    g_texture_gauntlet = LoadTexture("assets/images/manopla_estalo.png");
    g_sound_snap = LoadSound("assets/sounds/estalo_thanos.mp3"); 


    // Carrega perguntas do CSV (chamada única no início)
    printf("Carregando perguntas do arquivo CSV... (Verifique o console para feedback)\n");
    g_perguntas = carregaPerguntasDeCSV("questoes.csv", &g_total_perguntas);
    
    if (g_perguntas == NULL) {
        printf("AVISO: Nenhuma pergunta carregada ou erro ao abrir 'questoes.csv'.\n");
        g_total_perguntas = 0;
    } else {
        printf("Foram carregadas %d perguntas.\n", g_total_perguntas);
    }
    printf("Pressione ESC na janela Raylib para sair a qualquer momento.\n");


    // REPRODUZ MÚSICA DE FUNDO NO INÍCIO DO JOGO
    PlayMusicStream(g_music_background);


    // --- Loop Principal do Jogo (Raylib) ---
    // Este loop é o coração do jogo, chamando as funções de Update e Draw para a tela atual.
    while (!WindowShouldClose() && currentScreen != GAME_EXIT) { 
        // ATUALIZA O FLUXO DA MÚSICA DE FUNDO
        UpdateMusicStream(g_music_background);

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
            case GAME_PAUSE:            // UpdatePauseScreen();        break; // Implementar depois se necessário
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
                case GAME_PAUSE:            // DrawPauseScreen();        break; // Implementar depois
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
    // DESCARREGA TODOS OS RECURSOS DE ÁUDIO E TEXTURAS
    UnloadMusicStream(g_music_background);
    UnloadSound(g_sound_correct);
    UnloadSound(g_sound_wrong);
    UnloadSound(g_sound_win);
    UnloadSound(g_sound_lose);
    UnloadSound(g_sound_menu_click);
    UnloadTexture(g_texture_gauntlet); 
    UnloadSound(g_sound_snap);         
    CloseAudioDevice(); 
    CloseWindow();

    return 0;
}



