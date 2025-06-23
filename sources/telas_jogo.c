//Implementações para as Telas do Jogo Raylib

#include "telas_jogo.h" 
#include <stdio.h>    // Para printf, sprintf
#include <string.h>   // Para strlen, etc.
#include <ctype.h>    // Para toupper

// As variáveis globais (g_perguntas, currentScreen, etc.) são declaradas como 'extern' em telas_jogo.h.
// Suas definições reais estarão no main.c, que as inicializa.

// --- Implementações das Funções Auxiliares de UI Gráfica ---
// Funções que estavam no main.c e agora estão aqui
bool GuiButton(Rectangle bounds, const char *text, Color buttonColor, Color textColor) {
    bool clicked = false;
    Vector2 mousePoint = GetMousePosition();
    
    if (CheckCollisionPointRec(mousePoint, bounds)) {
        buttonColor = Fade(buttonColor, 0.7f);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            clicked = true;
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
    } else {
        printf("Nenhuma pergunta encontrada para o nivel %d! Voltando ao menu principal.\n", g_current_level);
        currentScreen = GAME_MAIN_MENU;
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
            printf("--- FIM DA PESQUISA NO CONSOLE ---\n");
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
        currentScreen = -1; // Sinaliza para o main() que é para sair do loop
    }
}

void UpdateDisplayQuestionsScreen(void) {
    const int screenWidth = GetScreenWidth(); // Pega a largura da tela atual
    const int screenHeight = GetScreenHeight(); // Pega a altura da tela atual

    if (IsKeyPressed(KEY_SPACE)) { // Pressione ESPAÇO para ir para a próxima pergunta
        g_current_display_question_idx++;
        if (g_current_display_question_idx >= g_total_perguntas) {
            g_current_display_question_idx = 0; // Reseta o índice
            currentScreen = GAME_MAIN_MENU;
        }
    }
    if (IsKeyPressed(KEY_ESCAPE)) { // Voltar ao menu principal com ESC
        currentScreen = GAME_MAIN_MENU;
    }
}

void UpdatePlayingGraphicalScreen(void) {
    const int screenWidth = GetScreenWidth(); // Pega a largura da tela atual
    const int screenHeight = GetScreenHeight(); // Pega a altura da tela atual

    if (g_current_question_idx == -1) {
        currentScreen = GAME_ENDING; 
        return;
    }

    Pergunta *current_question = &g_perguntas[g_current_question_idx];
    
    switch (g_game_play_state) {
        case PLAYING_QUESTION: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePoint = GetMousePosition();
                char selected = ' ';
                float optionY = 250;
                float optionHeight = 60;
                float optionPadding = 20;

                for (int i = 0; i < 4; i++) {
                    Rectangle optionBounds = {50, optionY + i * (optionHeight + optionPadding), screenWidth - 100, optionHeight};
                    if (CheckCollisionPointRec(mousePoint, optionBounds)) {
                        selected = current_question->alternativas[i].letra;
                        break;
                    }
                }
                if (selected != ' ') {
                    g_selected_answer_char = selected;
                    g_is_answer_correct = (toupper(selected) == toupper(current_question->correta));
                    g_game_play_state = PLAYING_FEEDBACK;
                    g_answer_feedback_timer = 0;
                }
            }
            if (IsKeyPressed(KEY_A)) { g_selected_answer_char = 'A'; g_is_answer_correct = (toupper('A') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; }
            if (IsKeyPressed(KEY_B)) { g_selected_answer_char = 'B'; g_is_answer_correct = (toupper('B') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; }
            if (IsKeyPressed(KEY_C)) { g_selected_answer_char = 'C'; g_is_answer_correct = (toupper('C') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; }
            if (IsKeyPressed(KEY_D)) { g_selected_answer_char = 'D'; g_is_answer_correct = (toupper('D') == toupper(current_question->correta)); g_game_play_state = PLAYING_FEEDBACK; g_answer_feedback_timer = 0; }

        } break;
        case PLAYING_FEEDBACK: {
            g_answer_feedback_timer++;
            if (g_answer_feedback_timer >= 60 * 2) { // Mostra feedback por 2 segundos (60 FPS * 2)
                if (g_is_answer_correct) {
                    g_correct_answers_in_row++;
                    if (g_correct_answers_in_row == 2 || g_correct_answers_in_row == 4 || g_correct_answers_in_row == 8 || g_correct_answers_in_row == 12 || g_correct_answers_in_row == 15) {
                        g_current_level++;
                        printf("Progrediu para o nível %d!\n", g_current_level);
                    }
                    if (g_correct_answers_in_row >= 15) {
                        currentScreen = GAME_WIN;
                    } else {
                        g_game_play_state = PLAYING_NEXT_QUESTION;
                    }
                } else {
                    currentScreen = GAME_LOSE;
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
    if (IsKeyPressed(KEY_ENTER)) currentScreen = GAME_MAIN_MENU;
}
void UpdateLoseScreen(void) {
    if (IsKeyPressed(KEY_ENTER)) currentScreen = GAME_MAIN_MENU;
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

        DrawText(TextFormat("NIVEL %d", g_current_level), 50, 20, 30, MARVEL_GOLD);
        DrawText(TextFormat("RESPOSTAS CORRETAS: %d", g_correct_answers_in_row), screenWidth - 400, 20, 20, MARVEL_LIGHTGRAY);
        
        DrawTextEx(GetFontDefault(), current_question->enunciado, 
                   (Vector2){50, 100}, 20, 2, RAYWHITE);
        
        float optionY = 250;
        float optionHeight = 60;
        float optionPadding = 20;

        for (int i = 0; i < 4; i++) {
            Rectangle optionBounds = {50, optionY + i * (optionHeight + optionPadding), screenWidth - 100, optionHeight};
            Color optionColor = MARVEL_BLUE;

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

        if (g_game_play_state == PLAYING_FEEDBACK) {
            if (g_is_answer_correct) {
                DrawText("CORRETO!", screenWidth/2 - MeasureText("CORRETO!", 50)/2, screenHeight - 100, 50, COLOR_CORRETO);
            } else {
                DrawText("ERRADO!", screenWidth/2 - MeasureText("ERRADO!", 50)/2, screenHeight - 100, 50, COLOR_ERRADO);
            }
        }
    } else {
        DrawText("Nenhuma pergunta disponivel para jogar.", screenWidth/2 - MeasureText("Nenhuma pergunta disponivel para jogar.", 20)/2, screenHeight/2, 20, RAYWHITE);
        DrawText("Pressione ESC para voltar ao menu", screenWidth/2 - MeasureText("Pressione ESC para voltar ao menu", 15)/2, screenHeight/2 + 30, 15, MARVEL_LIGHTGRAY);
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