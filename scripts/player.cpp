typedef struct Player 
{
    Texture2D sprite;
    Color color;
    float speed, jump_speed, gravity, position_x, position_y, size_multiplier;
} Player;




void DrawPlayer(Player player, int frame = 0, int animation_fps = 14)
{
    DrawTexturePro(player.sprite, {animation_fps ? ((int)(frame / (GAME_FPS / animation_fps) % 6)) * PLAYER_WIDTH : 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT},
                  {player.position_x, player.position_y, PLAYER_WIDTH * player.size_multiplier, PLAYER_HEIGHT * player.size_multiplier},
                  {0, 0}, (int)(player.speed * 1.33), player.color);
}



void InitializePlayer(Player *player, Difficulty difficulty)
{
    player->speed           = 0;
    player->jump_speed      = difficulty.player_jump_speed;
    player->gravity         = difficulty.player_gravity;
    player->position_x      = difficulty.player_position_x;
    player->position_y      = 300;
    player->size_multiplier = 1;
    player->color           = WHITE;
}



void UpdatePlayer(Player *player)
{
    player->speed      += player->gravity;
    player->position_y += player->speed;
}


void InitializeBackgroundBird(Player *background_bird)
{
    background_bird->position_x      = GetRandomValue(3, 6) * -120;
    background_bird->position_y      = GetRandomValue(70, 550);
    background_bird->speed           = 0;
    background_bird->size_multiplier = GetRandomValue(2, 5.5) * 0.1;
    background_bird->color           = WHITE;
}


void UpdateBackgroundBird(Player *background_bird)
{
    if (background_bird->position_x >= WINDOW_WIDTH + 200)
    {
        background_bird->position_x      = GetRandomValue(5, 15) * -120;
        background_bird->position_y      = GetRandomValue(70, 550);
        background_bird->size_multiplier = GetRandomValue(2, 5.5) * 0.1;
    }

    background_bird->position_x += 7.5 * background_bird->size_multiplier;
}
