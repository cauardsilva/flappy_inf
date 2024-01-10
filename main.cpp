#include "raylib.h"
#include <algorithm>
#include <math.h>
#include <iostream>

using namespace std;

enum MenuButtons {SINGLEPLAYERGAME = 0, MULTIPLAYERGAME = 1, RANKING = 2, SETTINGS = 3, EXITGAME = 4};

typedef struct Player 
{
    Texture2D sprite;
    int current_frame;
    float speed, acceleration, position_x, position_y;
} Player;



typedef struct Obstacle
{
    Texture2D sprite;
    float speed, acceleration, position_x, gap_height, gap_position_y;
} Obstacle;




typedef struct Config
{
    bool player_sound, ask_difficulty;
} Config;




void DrawObstacle(Obstacle obstacle)
{
    DrawTexturePro(obstacle.sprite, {0, 0, 200, 800}, {obstacle.position_x, obstacle.gap_position_y + (obstacle.gap_height / 2), 200, 800}, {100, 0}, 0,   WHITE);
    DrawTexturePro(obstacle.sprite, {0, 0, 200, 800}, {obstacle.position_x, obstacle.gap_position_y - (obstacle.gap_height / 2), 200, 800}, {100, 0}, 180, WHITE);
}


void DrawPlayer(Player* player, int frame, int animation_fps = 15, float size_multiplier = 1.0)
{
    if      ((frame % (60 / animation_fps) == 0) && (player->current_frame <  6)) ++player->current_frame;
    else if ((frame % (60 / animation_fps) == 0) && (player->current_frame == 6))   player->current_frame = 0;
    
    DrawTexturePro(player->sprite, {player->current_frame * 120, 0, 120, 96}, {96, player->position_y, 120 * size_multiplier, 96 * size_multiplier}, {player->position_x, 0}, min(14, (int)(player->speed * 1.33)), WHITE);
}

void StartSingleplayerGame()
{
    bool SingleplayerGameHasEnded = false;
    int frame = 0;
    
    
    Player player;
    player.sprite = LoadTexture("./resources/sprites/characters/queroquero.png");
    player.speed         = 10.0;
    player.position_y    = 20.0;
    player.current_frame = 0;
    

    Texture2D bg = LoadTexture("./resources/sprites/scenes/DESERT/DESERT.png");
    


    Obstacle obs;
    obs.sprite = LoadTexture("./resources/sprites/pipe.png");
    obs.speed = -8;
    obs.gap_height = 350;
    obs.gap_position_y = 400;
    obs.position_x = 1200;
    
    int vrau = 1;
    
    while ((!SingleplayerGameHasEnded) && (!WindowShouldClose()))
    {
        ///////////
        ++frame;
        ///////////
        
        if (obs.position_x > -100) obs.position_x += obs.speed;
        else 
        {
            obs.position_x = 1300;
            obs.gap_position_y = GetRandomValue(150, 650);
        }
        
        obs.gap_position_y += 2 * vrau;
        
        
        

        if (frame % 60 == 0) vrau = -vrau;
        
        
        
        if (IsKeyPressed(KEY_SPACE)) player.speed = -10;
        else                         player.speed += .3;
        
        player.position_y += player.speed;
        
        BeginDrawing();
            DrawTexture(bg, 0, 0, WHITE);
            DrawPlayer(&player, frame);
            DrawObstacle(obs);
        EndDrawing();
    }
}

int main(void)
{
    SetTargetFPS(60);
    
    InitWindow(1200, 800, "Flappy Quero Quero");
    
    
    int selector = 0;
    char strbuffer[128];
    
    Texture2D background = LoadTexture("./resources/sprites/scenes/menu.png");
    
    
    int frame = 0;
    Player player;
    player.sprite = LoadTexture("./resources/sprites/characters/queroquero.png");
    player.speed         = 0;
    player.position_y    = 600;
    player.position_x    = 100;
    player.current_frame = 0;
    
    
    while (!WindowShouldClose())
    {
        ++frame;
        player.position_x -= 8;
        
        if ((IsKeyPressed(KEY_RIGHT)) && ((MenuButtons)selector != EXITGAME))
            ++selector;

        if ((IsKeyPressed(KEY_LEFT)) && ((MenuButtons)selector != SINGLEPLAYERGAME))
            --selector;

        if (IsKeyPressed(KEY_ENTER))
        {
            switch ((MenuButtons)selector)
            {
                case SINGLEPLAYERGAME:
                    StartSingleplayerGame();
                    break;
                case EXITGAME:
                    return 0;
                default:
                    // this shouldnt be able to happen
                    break;
            }
        }
        
        BeginDrawing();
            DrawTexture(background, 0, 0, WHITE);
            DrawPlayer(&player, frame, 12, .3);
            sprintf(strbuffer, "Seletor: [%d]", (int)selector);
            DrawText(strbuffer, 600, 400, 30, RED);
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}






