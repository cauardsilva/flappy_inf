#include "global_constants.cpp"

#define PLAYER_WIDTH  120
#define PLAYER_HEIGHT 96

using namespace std;

typedef struct Player 
{
    Texture2D sprite;
    int current_frame;
    float speed, jump_speed, acceleration, position_x, position_y;
} Player;



// Draws a player and updates its current animation frame based on current game frame. There are optional parameters for customization.
void DrawPlayer(Player *player, int frame, int animation_fps = 12, float size_multiplier = 1.0, Color color = WHITE)
{
    if (animation_fps != 0)
    {
        if ((frame % (GAME_FPS / animation_fps) == 0) && (player->current_frame < 6))
            ++player->current_frame;
        else if ((frame % (GAME_FPS / animation_fps) == 0) && (player->current_frame == 6))
            player->current_frame = 0;
    }
    
    DrawTexturePro(player->sprite, {player->current_frame * PLAYER_WIDTH, 0, PLAYER_WIDTH, PLAYER_HEIGHT},
                  {player->position_x, player->position_y, PLAYER_WIDTH * size_multiplier, PLAYER_HEIGHT * size_multiplier},
                  {0, 0}, (int)(player->speed * 1.33), color);
}

void UpdatePlayer(Player *player)
{
    player->speed      += player->acceleration;
    player->position_y += player->speed;
}