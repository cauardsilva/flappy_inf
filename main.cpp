#include "raylib.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "./scripts/global_constants.cpp"
#include "./scripts/interactions.cpp"
#include "./scripts/singleplayer_game.cpp"
#include "./scripts/multiplayer_game.cpp"



using namespace std;


enum MenuButton {SINGLEPLAYERGAME = 0, MULTIPLAYERGAME = 1, RANKING = 2, SETTINGS = 3, EXITGAME = 4};


int main(void)
{
    SetTargetFPS(GAME_FPS);
    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Flappy Quero Quero");
    InitAudioDevice();
    
    
    
    Texture2D background = LoadTexture("./resources/sprites/scenes/menu.png");
    
    Player background_bird;
    background_bird.sprite        = LoadTexture("./resources/sprites/characters/queroquero.png");
    background_bird.speed         = 4.2;
    background_bird.position_y    = 500;
    background_bird.position_x    = WINDOW_WIDTH + 50;
    background_bird.current_frame = 0;
    float size = .3;
    
    Music background_music = LoadMusicStream("./resources/sounds/menu_background_music.mp3");
    SetMusicVolume(background_music, .001);
    
    
   
    int frame = 0;
    int selector = 0, difficulty_selector = 1;
    
    PlayMusicStream(background_music);
    
    while (!WindowShouldClose())
    {
        ////////
        ++frame;
        ////////
        
          
        if ((IsKeyPressed(KEY_RIGHT)) && ((MenuButton)selector != EXITGAME))         
            ++selector;
        if ((IsKeyPressed(KEY_LEFT)) && ((MenuButton)selector != SINGLEPLAYERGAME)) 
            --selector;
        
        if ((MenuButton)selector == SINGLEPLAYERGAME)
        {
            if ((IsKeyPressed(KEY_UP)) && ((GameDifficulty)difficulty_selector != CHILL)) 
                --difficulty_selector;
            if ((IsKeyPressed(KEY_DOWN)) && ((GameDifficulty)difficulty_selector != CUSTOM)) 
                ++difficulty_selector;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            switch ((MenuButton)selector)
            {
                case SINGLEPLAYERGAME:
                    StartSingleplayerGame(LoadDifficulty(difficulty_selector), LoadRanking());
                    break;
                case MULTIPLAYERGAME:
                    StartMultiplayerGame();
                    break;
                case EXITGAME:
                    return 0;
                default:
                    // This shouldn't happen
                    
                    break;
            }
        }
        
        
        
        
        if (background_bird.position_x >= WINDOW_WIDTH + 50)
        {
            // It will take the bird aproximately (5, 15) seconds to reach the screen from the new position
            background_bird.position_x = GetRandomValue(5, 15) * background_bird.speed * GAME_FPS * -1;
            background_bird.position_y = GetRandomValue(100, 500);
            size                       = GetRandomValue(2, 4) * 0.1;
        }
        background_bird.position_x += background_bird.speed;
        
        
        UpdateMusicStream(background_music);
        
        BeginDrawing();
            DrawTexture(background, 0, 0, WHITE);
            DrawPlayer(&background_bird, frame, 12, size);
            DrawText(TextFormat("Seletor: [%d]", selector), 600, 400, 30, RED);
            DrawText(TextFormat("Dificuldade: [%d]", difficulty_selector), 950, 100, 25, RED);
        EndDrawing();
    }
    
    
    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}






