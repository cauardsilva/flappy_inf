using namespace std;

#include "raylib.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <stdint.h>
#include <unistd.h>
#include <cmath>

#include "./scripts/global_constants.cpp"
#include "./scripts/utils.cpp"

#include "./scripts/difficulty.cpp"

#include "./scripts/player.cpp"
#include "./scripts/obstacle.cpp"
#include "./scripts/interactions.cpp"

#include "./scripts/ranking.cpp"

#include "./scripts/background.cpp"

#include "./scripts/singleplayer_game.cpp"
#include "./scripts/multiplayer_game.cpp"
#include "./scripts/ranking_screen.cpp"








enum MenuButton {SINGLEPLAYERGAME = 0, MULTIPLAYERGAME = 1, RANKING = 2, EXITGAME = 3};



int main(void)
{
    SetTargetFPS(GAME_FPS);
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flappy Quero Quero");
    InitAudioDevice();
    
    HideCursor();
    
    
    
    Texture2D background = LoadTexture("./resources/sprites/scenes/background.png");
    
    
    Texture2D menu_buttons[4];
    for (int i = 0; i < 4; ++i) 
        menu_buttons[i] = LoadTexture(TextFormat("./resources/sprites/ui/menu/buttons/%d.png", i));
    
    
    Texture2D title[3];
    for (int i = 0; i < 3; ++i)
        title[i] = LoadTexture(TextFormat("./resources/sprites/ui/menu/title/%d.png", i));
    
    float title_offset[1][2] = {-1, 0};
    
    
   
    
    
    Music background_music = LoadMusicStream("./resources/sounds/quero_quero_background.mp3");
    SetMusicVolume(background_music, .001);
    
    
    
    Player background_bird;
    background_bird.sprite = LoadTexture("./resources/sprites/characters/queroquero.png");
    InitializeBackgroundBird(&background_bird);
    

    
    int frame = 0;
    int selector = 0;
    
    PlayMusicStream(background_music);
    
    while (!WindowShouldClose())
    {
        ////////
        ++frame;
        ////////
        
        
        if ((IsKeyPressed(KEY_RIGHT)) && ((MenuButton)selector != EXITGAME))         ++selector;
        if ((IsKeyPressed(KEY_LEFT))  && ((MenuButton)selector != SINGLEPLAYERGAME)) --selector;
        
        
        if (IsKeyPressed(KEY_SPACE))
        {   
            switch ((MenuButton)selector)
            {
                case SINGLEPLAYERGAME:
                    StartSingleplayerGame();
                    break;
                case MULTIPLAYERGAME:
                    StartMultiplayerGame();
                    break;
                case RANKING:
                    ShowRankingScreen();
                    break;
                case EXITGAME:
                    return 0; // Change that, as CloseAudioDevice and CloseWindow should happen
                default:
                    // This shouldn't happen
                    break;
            }
        }
        
        
        
        title_offset[0][1] += 0.25 * title_offset[0][0];
            
        if      (title_offset[0][1] >=  5) title_offset[0][0] = -1;
        else if (title_offset[0][1] <= -5) title_offset[0][0] =  1;
        
        
        UpdateMusicStream(background_music);
        
        
        UpdateBackgroundBird(&background_bird);
        
        
        
        BeginDrawing();
        
            DrawTexture(background, 0, 0, WHITE);
            
            
            // Draws every menu button, highlighting the selected one
            for (int i = 0; i < 4; ++i)
                DrawTexture(menu_buttons[i], 165 + (265 * i), 655, selector == i ? WHITE : GRAY);
            
            
            
            // Lower part of the title: QUERO (the little one)
            DrawTexture(title[2], WINDOW_WIDTH/2 - 110, 450, WHITE);
           
            // Upper part of the title: FLAPPY
            DrawTexture(title[1], WINDOW_WIDTH/2 - 116, 200, WHITE);
            
            
            DrawPlayer(background_bird, frame);
           
           
            // Main part of the title: QUERO
            DrawTexture(title[0], WINDOW_WIDTH/2 - 393, 265 + title_offset[0][1], WHITE);
            
            
            
            // Footer
            DrawText("© 2024 Cauã Rodriguez da Silva. All rights reserved.", WINDOW_WIDTH/2 - MeasureText("© 2024 Caua Rodriguez da Silva. All rights reserved.", 20)/2, 750, 20, WHITE);
            
        EndDrawing();
    } 
    
    
    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}







