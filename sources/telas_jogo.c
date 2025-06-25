// sources/telas_jogo.c - Implementações para as Telas do Jogo Raylib (com Dicas, Posicionamento e Sons)

#include "Headers/telas_jogo.h" // Inclui o cabeçalho das telas para ver os protótipos e variáveis globais
#include <stdio.h>    // Para printf, sprintf
#include <string.h>   // Para strlen, etc.
#include <ctype.h>    // Para toupper
#include <stdlib.h>   // Para rand


// As variáveis globais (g_perguntas, currentScreen, etc.) são declaradas como 'extern' em telas_jogo.h.
// Suas definições reais (onde elas são inicializadas) estão no main.c.
// Aqui, nós apenas as usamos.


// --- Implementações das Funções Auxiliares de UI Gráfica ---
bool GuiButton(Rectangle bounds, const char *text, Color buttonColor, Color textColor) {
    bool clicked = false;
    Vector2 mousePoint = GetMousePosition();
    
    // Verifica se o mouse está sobre o botão
    if (CheckCollisionPointRec(mousePoint, bounds)) {
        buttonColor = Fade(buttonColor, 0.7f); // Escurece um pouco ao passar o mouse
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clicked = true;
            PlaySound(g_sound_menu_click); // Toca som de clique no menu
        }
    }
    
    DrawRectangleRec(bounds, buttonColor);
    DrawText(text, bounds.x + bounds.width/2 - MeasureText(text, 20)/2, bounds.y + bounds.height/2 - 10, 20, textColor);
    
    return clicked;
}

void ResetGamePlayingState(void) {
    g_current_level = 1;
    g_correct_answers_in_row = 0;
    g_current_question_idx = -1; 
    g_game_play_state = PLAYING_QUESTION;
    g_answer_feedback_timer = 0;
    g_is_answer_correct = false;
    g_selected_answer_char = ' ';
    g_correct_answer_char = ' ';
    
    // RESETAR CONTADORES DE DICAS
    g_hint_exclude_used = 0;
    g_hint_skip_used = 0;
    g_hint_fifty_fifty_used = 0;
    g_fifty_fifty_active = false; // Desativa a dica 50/50
    g_fifty_fifty_eliminated_chars[0] = 0; // Limpa caracteres eliminados
    g_fifty_fifty_eliminated_chars[1] = 0;
    
    // RESETAR ESTADO DA MANOPLA
    g_gauntlet_snap_active = false;
    g_gauntlet_snap_timer = 0;
}

void LoadNextQuestion(void) {
    Pergunta *question = sorteiaPorNivel(g_perguntas, g_total_perguntas, g_current_level);

    if (question != NULL) {
        for (int i = 0; i < g_total_perguntas; ++i) {
            if (&g_perguntas[i] == question) {
                g_current_question_idx = i;
                break;
            }
        }
        g_game_play_state = PLAYING_QUESTION;
        g_answer_feedback_timer = 0;
        g_is_answer_correct = false;
        g_selected_answer_char = ' ';
        g_correct_answer_char = question->correta;
        g_fifty_fifty_active = false; // Desativa 50/50 para a nova pergunta (IMPORTANT E)
        g_fifty_fifty_eliminated_chars[0] = 0; // Limpa caracteres eliminados
        g_fifty_fifty_eliminated_chars[1] = 0;
    } else {
        printf("Nenhuma pergunta encontrada para o nivel %d! Voltando ao menu principal.\n", g_current_level);
        currentScreen = GAME_MAIN_MENU; // Volta ao menu se não houver mais perguntas para o nível
    }
}


// --- Implementações das Funções de Update (Lógica de cada tela) ---
void UpdateLogoScreen(void) {
    framesCounter++;
    if (framesCounter > 180) currentScreen = GAME_TITLE;
}

void UpdateTitleScreen(void) {
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
        currentScreen = GAME_MAIN_MENU;
        PlaySound(g_sound_menu_click); // Som ao ir para o menu
    }
}

void UpdateMainMenuScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    float buttonWidth = screenWidth * 0.7f;
    float buttonHeight = 60;
    float startY = screenHeight / 4;
    float padding = 20;

    Rectangle playButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY, buttonWidth, buttonHeight };
    if (GuiButton(playButtonBounds, "1 - JOGAR", MARVEL_BLUE, RAYWHITE)) { 
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para jogar! Insira algumas primeiro (CONSOLE).\n");
        } else {
            ResetGamePlayingState();
            LoadNextQuestion();
            currentScreen = GAME_PLAYING_GRAPHICAL;
            StopMusicStream(g_music_background); // Para a música de fundo quando o jogo começa
        }
    }

    Rectangle insertButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + buttonHeight + padding, buttonWidth, buttonHeight };
    if (GuiButton(insertButtonBounds, "2 - INSERIR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
        printf("\n--- INSERINDO PERGUNTA (INTERAJA PELO CONSOLE) ---\n");
        recebePergunta(&g_perguntas, &g_total_perguntas);
        printf("--- FIM DA INSERCAO NO CONSOLE ---\n");
        currentScreen = GAME_MAIN_MENU;
    }
    
    Rectangle listButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(listButtonBounds, "3 - LISTAR PERGUNTAS", MARVEL_BLUE, RAYWHITE)) {
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para listar (CONSOLE).\n");
        } else {
            printf("\n--- LISTANDO PERGUNTAS (VERIFIQUE O CONSOLE) ---\n");
            listaPerguntas(g_perguntas, g_total_perguntas);
            printf("--- FIM DA LISTAGEM NO CONSOLE ---\n");
        }
        currentScreen = GAME_MAIN_MENU;
    }

    Rectangle searchButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(searchButtonBounds, "4 - PESQUISAR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
         if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para pesquisar (CONSOLE).\n");
        } else {
            printf("\n--- PESQUISANDO PERGUNTA NO CONSOLE ---\n");
            printf("A janela grafica pode congelar. Interaja pelo terminal.\n");
            pesquisaPergunta(g_perguntas, g_total_perguntas);
            printf("--- FIM DELLA RICERCA NEL CONSOLE ---\n");
        }
        currentScreen = GAME_MAIN_MENU;
    }

    Rectangle alterButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 4 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(alterButtonBounds, "5 - ALTERAR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para alterar (CONSOLE).\n");
        } else {
            printf("\n--- ALTERANDO PERGUNTA NO CONSOLE ---\n");
            printf("A janela grafica pode congelar. Interaja pelo terminal.\n");
            alterarPergunta(g_perguntas, g_total_perguntas);
            printf("--- FIM DA ALTERACAO NO CONSOLE ---\n");
        }
        currentScreen = GAME_MAIN_MENU;
    }

    Rectangle deleteButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 5 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(deleteButtonBounds, "6 - EXCLUIR PERGUNTA", MARVEL_BLUE, RAYWHITE)) {
        if (g_total_perguntas == 0) {
            printf("Nao ha perguntas para excluir (CONSOLE).\n");
        } else {
            printf("\n--- EXCLUINDO PERGUNTA NO CONSOLE ---\n");
            printf("A janela grafica pode congelar. Interaja pelo terminal.\n");
            excluirPergunta(&g_perguntas, &g_total_perguntas);
            printf("--- FIM DA EXCLUSAO NO CONSOLE ---\n");
        }
        currentScreen = GAME_MAIN_MENU;
    }

    Rectangle exitButtonBounds = { (screenWidth - buttonWidth) / 2.0f, startY + 6 * (buttonHeight + padding), buttonWidth, buttonHeight };
    if (GuiButton(exitButtonBounds, "0 - SAIR", MARVEL_RED, RAYWHITE)) {
        currentScreen = GAME_EXIT;
    }
}

void UpdateDisplayQuestionsScreen(void) {
    if (IsKeyPressed(KEY_SPACE)) {
        g_current_display_question_idx++;
        if (g_current_display_question_idx >= g_total_perguntas) {
            g_current_display_question_idx = 0;
            currentScreen = GAME_MAIN_MENU;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = GAME_MAIN_MENU;
    }
}

void UpdatePlayingGraphicalScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    if (g_current_question_idx == -1) {
        currentScreen = GAME_ENDING; 
        return;
    }

    Pergunta *current_question = &g_perguntas[g_current_question_idx];
    
    // Lógica da Manopla do Estalo - LÓGICA DE ATUALIZAÇÃO DO TIMER
    if (g_gauntlet_snap_active) {
        g_gauntlet_snap_timer++;
        if (g_gauntlet_snap_timer >= 60 * 1) { // Exibe por 1 segundo (60 frames)
            g_gauntlet_snap_active = false;
            g_gauntlet_snap_timer = 0;
        }
    }

    switch (g_game_play_state) {
        case PLAYING_QUESTION: {
            // Lógica para DICAS (processamento do clique)
            float hintButtonWidth = 100;
            float hintButtonHeight = 40;
            float hintPadding = 10;
            float hintX_Rightmost = screenWidth - hintButtonWidth - 20; 
            float hintX_Middle = hintX_Rightmost - hintButtonWidth - hintPadding;
            float hintX_Leftmost = hintX_Middle - hintButtonWidth - hintPadding;
            float hintStartY = 50; 

            // Dica: Meio a Meio (50/50)
            Rectangle fiftyFiftyButtonBounds = { hintX_Leftmost, hintStartY, hintButtonWidth, hintButtonHeight };
            Color fiftyFiftyColor = (g_hint_fifty_fifty_used < 3) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
            if (GuiButton(fiftyFiftyButtonBounds, "50/50", fiftyFiftyColor, RAYWHITE)) {
                if (g_hint_fifty_fifty_used < 3 && !g_fifty_fifty_active) {
                    g_hint_fifty_fifty_used++;
                    g_fifty_fifty_active = true;

                    char wrong_alternatives[3]; 
                    int wrong_count = 0;
                    for (int i = 0; i < 4; i++) {
                        if (toupper(current_question->alternativas[i].letra) != toupper(current_question->correta)) {
                            wrong_alternatives[wrong_count++] = current_question->alternativas[i].letra;
                        }
                    }

                    if (wrong_count >= 2) {
                        int idx1 = rand() % wrong_count;
                        char eliminated1 = wrong_alternatives[idx1];

                        int idx2 = rand() % wrong_count;
                        while (idx2 == idx1) {
                            idx2 = rand() % wrong_count;
                        }
                        char eliminated2 = wrong_alternatives[idx2];
                        
                        g_fifty_fifty_eliminated_chars[0] = eliminated1;
                        g_fifty_fifty_eliminated_chars[1] = eliminated2;
                    } else {
                        g_fifty_fifty_active = false;
                    }
                    PlaySound(g_sound_snap); // Toca o som do estalo
                    g_gauntlet_snap_active = true; // Ativa a exibição da manopla
                    g_gauntlet_snap_timer = 0; // Reinicia o timer
                }
            }

            // Botão "Excluir Questão"
            Rectangle excludeButtonBounds = { hintX_Middle, hintStartY, hintButtonWidth, hintButtonHeight };
            Color excludeColor = (g_hint_exclude_used == 0) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
            if (GuiButton(excludeButtonBounds, "EXCLUIR", excludeColor, RAYWHITE)) {
                if (g_hint_exclude_used == 0) {
                    g_hint_exclude_used = 1;
                    g_game_play_state = PLAYING_NEXT_QUESTION;
                    PlaySound(g_sound_snap); // Toca o som do estalo
                    g_gauntlet_snap_active = true; // Ativa a exibição da manopla
                    g_gauntlet_snap_timer = 0; // Reinicia o timer
                }
            }

            // Botão "Pular Questão"
            Rectangle skipButtonBounds = { hintX_Rightmost, hintStartY, hintButtonWidth, hintButtonHeight };
            Color skipColor = (g_hint_skip_used < 2) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
            if (GuiButton(skipButtonBounds, "PULAR", skipColor, RAYWHITE)) {
                if (g_hint_skip_used < 2) {
                    g_hint_skip_used++;
                    g_game_play_state = PLAYING_NEXT_QUESTION;
                    PlaySound(g_sound_snap); // Toca o som do estalo
                    g_gauntlet_snap_active = true; // Ativa a exibição da manopla
                    g_gauntlet_snap_timer = 0; // Reinicia o timer
                }
            }


            // Detecta cliques nas alternativas
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePoint = GetMousePosition();
                char selected = ' ';
                float optionY = 250;
                float optionHeight = 60;
                float optionPadding = 20;

                for (int i = 0; i < 4; i++) {
                    bool is_eliminated = false;
                    if (g_fifty_fifty_active) {
                        if (toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[0]) ||
                            toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[1])) {
                            is_eliminated = true;
                        }
                    }

                    if (!is_eliminated) {
                        Rectangle optionBounds = {50, optionY + i * (optionHeight + optionPadding), screenWidth - 100, optionHeight};
                        if (CheckCollisionPointRec(mousePoint, optionBounds)) {
                            selected = current_question->alternativas[i].letra;
                            break;
                        }
                    }
                }
                if (selected != ' ') {
                    g_selected_answer_char = selected;
                    g_is_answer_correct = (toupper(selected) == toupper(current_question->correta));
                    g_game_play_state = PLAYING_FEEDBACK;
                    g_answer_feedback_timer = 0;
                    // Toca som de acerto/erro
                    if (g_is_answer_correct) PlaySound(g_sound_correct);
                    else PlaySound(g_sound_wrong);
                }
            }
            // Teclas A, B, C, D (AJUSTADO PARA 50/50)
            if (IsKeyPressed(KEY_A) && !(g_fifty_fifty_active && (toupper('A') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('A') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'A'; g_is_answer_correct = (toupper('A') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }
            if (IsKeyPressed(KEY_B) && !(g_fifty_fifty_active && (toupper('B') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('B') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'B'; g_is_answer_correct = (toupper('B') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }
            if (IsKeyPressed(KEY_C) && !(g_fifty_fifty_active && (toupper('C') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('C') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'C'; g_is_answer_correct = (toupper('C') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }
            if (IsKeyPressed(KEY_D) && !(g_fifty_fifty_active && (toupper('D') == toupper(g_fifty_fifty_eliminated_chars[0]) || toupper('D') == toupper(g_fifty_fifty_eliminated_chars[1])))) { g_selected_answer_char = 'D'; g_is_answer_correct = (toupper('D') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; if (g_is_answer_correct) PlaySound(g_sound_correct); else PlaySound(g_sound_wrong); }

        } break;
        case PLAYING_FEEDBACK: {
            g_answer_feedback_timer++;
            if (g_answer_feedback_timer >= 60 * 2) { 
                if (g_is_answer_correct) {
                    g_correct_answers_in_row++;
                    if (g_correct_answers_in_row == 2 || g_correct_answers_in_row == 4 || g_correct_answers_in_row == 8 || g_correct_answers_in_row == 12 || g_correct_answers_in_row == 15) {
                        g_current_level++;
                        printf("Progrediu para o nível %d!\n", g_current_level);
                    }
                    if (g_correct_answers_in_row >= 15) {
                        currentScreen = GAME_WIN;
                        PlaySound(g_sound_win); // Som de vitória
                    } else {
                        g_game_play_state = PLAYING_NEXT_QUESTION;
                    }
                } else {
                    currentScreen = GAME_LOSE;
                    PlaySound(g_sound_lose); // Som de derrota
                }
            }
        } break;
        case PLAYING_NEXT_QUESTION: {
            LoadNextQuestion();
        } break;
    }
}

void UpdateInsertQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) currentScreen = GAME_MAIN_MENU;
}
void UpdateListQuestionsScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) currentScreen = GAME_MAIN_MENU;
}
void UpdateSearchQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) currentScreen = GAME_MAIN_MENU;
}
void UpdateAlterQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) currentScreen = GAME_MAIN_MENU;
}
void UpdateDeleteQuestionScreen(void) {
    if (IsKeyPressed(KEY_ESCAPE)) currentScreen = GAME_MAIN_MENU;
}
void UpdateEndingScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) currentScreen = GAME_MAIN_MENU;
}
void UpdateWinScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) currentScreen = GAME_ENDING;
}
void UpdateLoseScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) currentScreen = GAME_ENDING;
}


// --- Implementações das Funções de Draw (Desenho de cada tela) ---
void DrawLogoScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "MARVEL - O DESAFIO", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "MARVEL - O DESAFIO", 40, 0).x/2, screenHeight/2 - 50}, 
               40, 0, MARVEL_RED);
    DrawTextEx(g_marvel_font, "Preparando o Universo...", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "Preparando o Universo...", 20, 0).x/2, screenHeight/2 + 10}, 
               20, 0, MARVEL_LIGHTGRAY);
}

void DrawTitleScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "SHOW DO MILHAO", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "SHOW DO MILHAO", 80, 0).x/2, screenHeight/4}, 
               80, 0, MARVEL_GOLD);
    DrawTextEx(g_marvel_font, "MARVEL", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "MARVEL", 60, 0).x/2, screenHeight/4 + 80}, 
               60, 0, MARVEL_RED);
    DrawText("Pressione ENTER para comecar", screenWidth/2 - MeasureText("Pressione ENTER para comecar", 20)/2, screenHeight/2 + 100, 20, MARVEL_LIGHTGRAY);
}

void DrawMainMenuScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("MENU PRINCIPAL", screenWidth/2 - MeasureText("MENU PRINCIPAL", 40)/2, screenHeight/8, 40, MARVEL_GOLD);
    float buttonWidth = screenWidth * 0.7f;
    float buttonHeight = 60;
    float startY = screenHeight / 4;
    float padding = 20;

    Rectangle rect1 = { (screenWidth - buttonWidth) / 2.0f, startY, buttonWidth, buttonHeight };
    GuiButton(rect1, "1 - JOGAR", MARVEL_BLUE, RAYWHITE);

    Rectangle rect2 = { (screenWidth - buttonWidth) / 2.0f, startY + buttonHeight + padding, buttonWidth, buttonHeight };
    GuiButton(rect2, "2 - INSERIR PERGUNTA", MARVEL_BLUE, RAYWHITE);

    Rectangle rect3 = { (screenWidth - buttonWidth) / 2.0f, startY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight };
    GuiButton(rect3, "3 - LISTAR PERGUNTAS", MARVEL_BLUE, RAYWHITE);

    Rectangle rect4 = { (screenWidth - buttonWidth) / 2.0f, startY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight };
    GuiButton(rect4, "4 - PESQUISAR PERGUNTA", MARVEL_BLUE, RAYWHITE);

    Rectangle rect5 = { (screenWidth - buttonWidth) / 2.0f, startY + 4 * (buttonHeight + padding), buttonWidth, buttonHeight };
    GuiButton(rect5, "5 - ALTERAR PERGUNTA", MARVEL_BLUE, RAYWHITE);

    Rectangle rect6 = { (screenWidth - buttonWidth) / 2.0f, startY + 5 * (buttonHeight + padding), buttonWidth, buttonHeight };
    GuiButton(rect6, "6 - EXCLUIR PERGUNTA", MARVEL_BLUE, RAYWHITE);

    Rectangle rect7 = { (screenWidth - buttonWidth) / 2.0f, startY + 6 * (buttonHeight + padding), buttonWidth, buttonHeight };
    GuiButton(rect7, "0 - SAIR", MARVEL_RED, RAYWHITE);
}

void DrawDisplayQuestionsScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    if (g_total_perguntas > 0 && g_current_display_question_idx < g_total_perguntas) {
        Pergunta *currentQuestion = &g_perguntas[g_current_display_question_idx];

        DrawText(TextFormat("PERGUNTA %d (NIVEL: %d)", g_current_display_question_idx + 1, currentQuestion->nivel), 50, 50, 25, MARVEL_GOLD);
        DrawTextEx(GetFontDefault(), currentQuestion->enunciado, 
                   (Vector2){50, 100}, 20, 2, RAYWHITE);

        float altStartY = 350;
        for (int i = 0; i < 4; i++) {
            char altText[256];
            sprintf(altText, "%c) %s", currentQuestion->alternativas[i].letra, currentQuestion->alternativas[i].texto);
            
            Color altColor = RAYWHITE;
            if (toupper(currentQuestion->alternativas[i].letra) == toupper(currentQuestion->correta)) {
                altColor = COLOR_CORRETO;
            }
            DrawText(altText, 50, altStartY + (i * 30), 20, altColor);
        }

        DrawText("Pressione ESPAÇO para a proxima pergunta, ESC para voltar ao menu.", 
                 50, screenHeight - 40, 15, MARVEL_LIGHTGRAY);

    } else {
        DrawText("Nenhuma pergunta para exibir. Volte ao menu.", screenWidth/2 - MeasureText("Nenhuma pergunta para exibir. Volte ao menu.", 20)/2, screenHeight/2, 20, RAYWHITE);
        DrawText("Pressione ESC para voltar ao menu.", screenWidth/2 - MeasureText("Pressione ESC para voltar ao menu.", 15)/2, screenHeight/2 + 30, 15, MARVEL_LIGHTGRAY);
    }
}

void DrawPlayingGraphicalScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    if (g_current_question_idx != -1) {
        Pergunta *current_question = &g_perguntas[g_current_question_idx];

        // Desenhar Nível e Respostas Corretas
        DrawText(TextFormat("NIVEL %d", g_current_level), 50, 20, 30, MARVEL_GOLD);
        DrawText(TextFormat("RESPOSTAS CORRETAS: %d", g_correct_answers_in_row), screenWidth - 300, 20, 20, MARVEL_LIGHTGRAY); 
        
        // Desenhar Enunciado da Pergunta
        DrawTextEx(GetFontDefault(), current_question->enunciado, 
                   (Vector2){50, 100}, 20, 2, RAYWHITE);
        
        // Desenhar Alternativas
        float optionY = 250;
        float optionHeight = 60;
        float optionPadding = 20;

        for (int i = 0; i < 4; i++) {
            Rectangle optionBounds = {50, optionY + i * (optionHeight + optionPadding), screenWidth - 100, optionHeight};
            Color optionColor = MARVEL_BLUE;

            bool is_eliminated_by_fifty_fifty = false;
            if (g_fifty_fifty_active) {
                if (toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[0]) ||
                    toupper(current_question->alternativas[i].letra) == toupper(g_fifty_fifty_eliminated_chars[1])) {
                    is_eliminated_by_fifty_fifty = true;
                }
            }

            if (is_eliminated_by_fifty_fifty) {
                optionColor = DARKGRAY;
                DrawRectangleRec(optionBounds, optionColor);
                DrawText("X", optionBounds.x + optionBounds.width / 2 - 10, optionBounds.y + optionHeight / 2 - 10, 20, RAYWHITE);
            } else {
                if (g_game_play_state == PLAYING_FEEDBACK) {
                    if (toupper(current_question->alternativas[i].letra) == toupper(g_selected_answer_char)) {
                        optionColor = g_is_answer_correct ? COLOR_CORRETO : COLOR_ERRADO;
                    } else if (toupper(current_question->alternativas[i].letra) == toupper(current_question->correta)) {
                        optionColor = COLOR_CORRETO;
                    }
                }
                DrawRectangleRec(optionBounds, optionColor);
                DrawText(TextFormat("%c) %s", current_question->alternativas[i].letra, current_question->alternativas[i].texto), 
                         optionBounds.x + 15, optionBounds.y + optionHeight/2 - 10, 20, RAYWHITE);
            }
        }

        // Mensagem de feedback (CORRETO/ERRADO)
        if (g_game_play_state == PLAYING_FEEDBACK) {
            if (g_is_answer_correct) {
                DrawText("CORRETO!", screenWidth/2 - MeasureText("CORRETO!", 50)/2, screenHeight - 100, 50, COLOR_CORRETO);
            } else {
                DrawText("ERRADO!", screenWidth/2 - MeasureText("ERRADO!", 50)/2, screenHeight - 100, 50, COLOR_ERRADO);
            }
        }

        // --- Desenhar botões de dica e seus contadores ---
        float hintButtonWidth = 100;
        float hintButtonHeight = 40;
        float hintPadding = 10;
        float hintX_Rightmost = screenWidth - hintButtonWidth - 20; 
        float hintX_Middle = hintX_Rightmost - hintButtonWidth - hintPadding;
        float hintX_Leftmost = hintX_Middle - hintButtonWidth - hintPadding;
        float hintStartY = 50; 
        float hintTextOffset = hintButtonHeight + 5; 
        
        // Dica: Meio a Meio
        Rectangle fiftyFiftyHintBounds = {hintX_Leftmost, hintStartY, hintButtonWidth, hintButtonHeight};
        Color fiftyFiftyHintColor = (g_hint_fifty_fifty_used < 3) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
        DrawRectangleRec(fiftyFiftyHintBounds, fiftyFiftyHintColor);
        DrawText("50/50", fiftyFiftyHintBounds.x + hintButtonWidth/2 - MeasureText("50/50", 20)/2, fiftyFiftyHintBounds.y + hintButtonHeight/2 - 10, 20, RAYWHITE);
        DrawText(TextFormat("%d/3", 3 - g_hint_fifty_fifty_used), fiftyFiftyHintBounds.x + hintButtonWidth / 2 - MeasureText(TextFormat("%d/3", 3 - g_hint_fifty_fifty_used), 15) / 2, fiftyFiftyHintBounds.y + hintTextOffset, 15, RAYWHITE);

        // Dica: Excluir Questão
        Rectangle excludeButtonBounds = {hintX_Middle, hintStartY, hintButtonWidth, hintButtonHeight};
        Color excludeHintColor = (g_hint_exclude_used == 0) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
        DrawRectangleRec(excludeButtonBounds, excludeHintColor);
        DrawText("EXCLUIR", excludeButtonBounds.x + hintButtonWidth/2 - MeasureText("EXCLUIR", 20)/2, excludeButtonBounds.y + hintButtonHeight/2 - 10, 20, RAYWHITE);
        DrawText(TextFormat("%d/1", 1 - g_hint_exclude_used), excludeButtonBounds.x + hintButtonWidth / 2 - MeasureText(TextFormat("%d/1", 1 - g_hint_exclude_used), 15) / 2, excludeButtonBounds.y + hintTextOffset, 15, RAYWHITE);

        // Dica: Pular Questão
        Rectangle skipButtonBounds = {hintX_Rightmost, hintStartY, hintButtonWidth, hintButtonHeight};
        Color skipHintColor = (g_hint_skip_used < 2) ? COLOR_HINT_AVAILABLE_PURPLE : COLOR_HINT_USED_PURPLE;
        DrawRectangleRec(skipButtonBounds, skipHintColor);
        DrawText("PULAR", skipButtonBounds.x + hintButtonWidth/2 - MeasureText("PULAR", 20)/2, skipButtonBounds.y + hintButtonHeight/2 - 10, 20, RAYWHITE);
        DrawText(TextFormat("%d/2", 2 - g_hint_skip_used), skipButtonBounds.x + hintButtonWidth / 2 - MeasureText(TextFormat("%d/2", 2 - g_hint_skip_used), 15) / 2, skipButtonBounds.y + hintTextOffset, 15, RAYWHITE);

        // DESENHAR MANOPLA DO ESTALO
        if (g_gauntlet_snap_active) {
            int gauntletPosX = (screenWidth - g_texture_gauntlet.width) / 2;
            int gauntletPosY = (screenHeight - g_texture_gauntlet.height) / 2;
            DrawTexture(g_texture_gauntlet, gauntletPosX, gauntletPosY, WHITE);
        }

    } else {
        // Se não há pergunta válida
        DrawText("Nenhuma pergunta disponivel para jogar.", screenWidth/2 - MeasureText("Nenhuma pergunta disponivel para jogar.", 20)/2, screenHeight/2, 20, RAYWHITE);
        DrawText("Pressione ESC para voltar ao menu", screenWidth/2 - MeasureText("Pressione ESC para voltar ao menu.", 15)/2, screenHeight/2 + 30, 15, MARVEL_LIGHTGRAY);
    }
}


void DrawInsertQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawListQuestionsScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawSearchQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawAlterQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawDeleteQuestionScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("FUNCAO EXECUTANDO NO CONSOLE...", screenWidth/2 - MeasureText("FUNCAO EXECUTANDO NO CONSOLE...", 20)/2, screenHeight/2 - 20, 20, MARVEL_LIGHTGRAY);
    DrawText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", screenWidth/2 - MeasureText(">>> INTERAJA PELO CONSOLE/TERMINAL <<<", 20)/2, screenHeight/2 + 10, 20, MARVEL_LIGHTGRAY);
    DrawText("Ao final, a janela voltara ao menu", screenWidth/2 - MeasureText("Ao final, a janela voltara ao menu", 15)/2, screenHeight/2 + 40, 15, MARVEL_LIGHTGRAY);
}
void DrawEndingScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawText("ESTADO FINAL/VITORIA/DERROTA (Pressione ENTER para menu)", screenWidth/2 - MeasureText("ESTADO FINAL/VITORIA/DERROTA (Pressione ENTER para menu)", 20)/2, screenHeight/2, 20, MARVEL_RED);
}
void DrawWinScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "VOCE GANHOU O JOGO DO MILHAO MARVEL!", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "VOCE GANHOU O JOGO DO MILHAO MARVEL!", 40, 0).x/2, screenHeight/3}, 
               40, 0, MARVEL_GOLD);
    DrawTextEx(g_marvel_font, "Parabens, Vingador!", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "Parabens, Vingador!", 30, 0).x/2, screenHeight/3 + 60}, 
               30, 0, MARVEL_LIGHTGRAY);
    DrawText("Pressione ENTER para voltar ao menu", screenWidth/2 - MeasureText("Pressione ENTER para voltar ao menu", 20)/2, screenHeight/2 + 100, 20, MARVEL_LIGHTGRAY);
}
void DrawLoseScreen(void) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    DrawTextEx(g_marvel_font, "VOCE PERDEU! Tente novamente.", 
               (Vector2){screenWidth/2 - MeasureTextEx(g_marvel_font, "VOCE PERDEU! Tente novamente.", 40, 0).x/2, screenHeight/3}, 
               40, 0, MARVEL_RED);
    DrawText("A resposta correta era: ", screenWidth/2 - MeasureText("A resposta correta era: ", 25)/2, screenHeight/3 + 60, 25, MARVEL_LIGHTGRAY);
    DrawText(TextFormat("Alternativa %c", toupper(g_correct_answer_char)), screenWidth/2 - MeasureText(TextFormat("Alternativa %c", toupper(g_correct_answer_char)), 40)/2, screenHeight/3 + 100, 40, MARVEL_GOLD);
    DrawText("Pressione ENTER para voltar ao menu", screenWidth/2 - MeasureText("Pressione ENTER para voltar ao menu", 20)/2, screenHeight/2 + 100, 20, MARVEL_LIGHTGRAY);
}
