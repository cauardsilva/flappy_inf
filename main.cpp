#include "raylib.h"
#include <iostream>
#include <fstream>

// Flappy Quero Quero is a very customizable version of Flappy Bird

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define GAME_FPS 60

#define PLAYER_WIDTH  120
#define PLAYER_HEIGHT 96

#define OBSTACLE_WIDTH  200
#define OBSTACLE_HEIGHT 800

#define OBSTACLE_QUANTITY (WINDOW_WIDTH / OBSTACLE_WIDTH) + 1

#define RANKING_QUANTITY 5




using namespace std;

enum MenuButton {SINGLEPLAYERGAME = 0, MULTIPLAYERGAME = 1, RANKING = 2, SETTINGS = 3, EXITGAME = 4};

typedef struct Player 
{
    Texture2D sprite;
    int current_frame;
    float speed, jump_speed, acceleration, position_x, position_y;
} Player;



typedef struct Obstacle
{
    Texture2D sprite;
    float speed, acceleration, position_x, gap_height, gap_position_y, gap_position_delta;
} Obstacle;



// CHILL:          Just sit down and relax.
// NORMAL:         The base Flappy Quero Quero experience.
// HARD:           For those who need a bigger challenge.
// INSANITY:       Good luck. You'll need it!
// INFINITY PIPES: Your fingers will need some rest after this one.
// CUSTOM:         Go crazy and create your own game mode.
enum GameDifficulty {CHILL = 0, NORMAL = 1, HARD = 2, INSANITY = 3, INFINITY = 4, CUSTOM = 5};

typedef struct Difficulty
{
    unsigned int score_threshold;
    float initial_gap_height,         gap_height_increment;
    float initial_gap_position_delta, gap_position_delta_increment;
    float initial_obstacle_speed,     obstacle_speed_increment;
    float initial_obstacle_distance;
    float initial_player_position_x;
    float initial_player_jump_speed;
    float initial_player_acceleration;
    

} Difficulty;


const Difficulty DEFAULT =
{
    10,       // score_threshold
    
    350, 0, // gap_height
    800, 0, // gap_position_delta
    6, 0,   // obstacle_speed
    400,    // obstacle_distance
    
    96,     // player_position_x
    10,     // player_jump_speed
    .3      // player_acceleration
};

Difficulty LoadDifficulty(int difficulty_selector)
{
    ifstream difficulty_file;
    
    if ((GameDifficulty)difficulty_selector != CUSTOM)
    {
        char directory[64];
        sprintf(directory, "./data/difficulties/difficulty%d.txt", difficulty_selector);
        difficulty_file.open(directory);
    }
    else difficulty_file.open("./data/difficulties/custom_difficulty.txt");
    
    float difficulty_settings[11];
    Difficulty difficulty;
    
    if ((difficulty_file.is_open()) && (!difficulty_file.peek() == ifstream::traits_type::eof()) )
    {
        for (int i = 0; i < 11; ++i)
            difficulty_file >> difficulty_settings[i];
        
        //difficulty = Difficulty(difficulty_settings);
        /*
            10,       // score_threshold
    
            350, 0, // gap_height
            800, 0, // gap_position_delta
            6, 0,   // obstacle_speed
            400,    // obstacle_distance
            
            96,     // player_position_x
            10,     // player_jump_speed
            .3      // player_acceleration
        
        */
        
        difficulty.score_threshold              = (unsigned int)difficulty_settings[0];
        
        difficulty.initial_gap_height           = difficulty_settings[1];
        
        difficulty.gap_height_increment         = difficulty_settings[2];
        
        difficulty.initial_gap_position_delta   = difficulty_settings[3];
        
        difficulty.gap_position_delta_increment = difficulty_settings[4];
        
        difficulty.initial_obstacle_speed       = difficulty_settings[5];
        
        difficulty.obstacle_speed_increment     = difficulty_settings[6];
        
        difficulty.initial_obstacle_distance    = difficulty_settings[7];
        
        difficulty.initial_player_position_x    = difficulty_settings[8];
        
        difficulty.initial_player_jump_speed    = difficulty_settings[9];
        
        difficulty.initial_player_acceleration  = difficulty_settings[10];
        
        
        difficulty_file.close();
        
        return difficulty;
    }
    else return DEFAULT;
}





char str_buffer[128];



// Updates all obstacles (on a list) based on game data (if provided), such as score and difficulty
void UpdateObstacles(Obstacle *obs_list, int *score, Difficulty difficulty)
{
    int num_obs = (WINDOW_WIDTH / OBSTACLE_WIDTH) + 1;
    for (int i = 0; i < num_obs; ++i)
    {
        obs_list[i].speed              = difficulty.initial_obstacle_speed     + (difficulty.obstacle_speed_increment     * (int)(*score / difficulty.score_threshold));
        obs_list[i].gap_height         = difficulty.initial_gap_height         - (difficulty.gap_height_increment         * (int)(*score / difficulty.score_threshold));
        obs_list[i].gap_position_delta = difficulty.initial_gap_position_delta + (difficulty.gap_position_delta_increment * (int)(*score / difficulty.score_threshold));
        
        if (obs_list[i].position_x > -100)
            obs_list[i].position_x -= obs_list[i].speed; 
        else 
        {
            obs_list[i].position_x = num_obs * (OBSTACLE_WIDTH + difficulty.initial_obstacle_distance) - (OBSTACLE_WIDTH / 2);
            
            int gap_delta_aux  = obs_list[i].gap_position_delta / 2;
            int gap_height_aux = obs_list[i].gap_height         / 2;
            if (i > 0) obs_list[i].gap_position_y = GetRandomValue(max(gap_height_aux, (int)(obs_list[i-1].gap_position_y       - gap_delta_aux)), min(WINDOW_HEIGHT - gap_height_aux, (int)(obs_list[i-1].gap_position_y       + gap_delta_aux)));
            else       obs_list[i].gap_position_y = GetRandomValue(max(gap_height_aux, (int)(obs_list[num_obs-1].gap_position_y - gap_delta_aux)), min(WINDOW_HEIGHT - gap_height_aux, (int)(obs_list[num_obs-1].gap_position_y + gap_delta_aux)));
            
            if (score != NULL) *score += 1;
        }
    }
}

// Checks if a player has collided (with some tolerance) with any obstacle inside a list
bool CheckPlayerCollision(Player player, Obstacle* obs_list)
{
    Rectangle player_rect = {player.position_x + 5, player.position_y + 5, PLAYER_WIDTH - 10, PLAYER_HEIGHT - 10};
    
    if ((player.position_y >= WINDOW_HEIGHT + 30) || (player.position_y <= -150)) return true;
    
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        if ((CheckCollisionRecs(player_rect, {obs_list[i].position_x - 95, obs_list[i].gap_position_y + (obs_list[i].gap_height / 2),                   OBSTACLE_WIDTH, OBSTACLE_HEIGHT})) ||
            (CheckCollisionRecs(player_rect, {obs_list[i].position_x - 95, obs_list[i].gap_position_y - (obs_list[i].gap_height / 2) - OBSTACLE_HEIGHT, OBSTACLE_WIDTH, OBSTACLE_HEIGHT}))) return true;
    
    return false;
}

// Draws both (upper and lower) portions of an obstacle
void DrawObstacle(Obstacle obstacle)
{
    DrawTexturePro(obstacle.sprite, {0, 0, OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {obstacle.position_x, obstacle.gap_position_y + (obstacle.gap_height / 2), OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {OBSTACLE_WIDTH / 2, 0}, 0,   WHITE);
    DrawTexturePro(obstacle.sprite, {0, 0, OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {obstacle.position_x, obstacle.gap_position_y - (obstacle.gap_height / 2), OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {OBSTACLE_WIDTH / 2, 0}, 180, WHITE);
}


// Draws a player and updates its current animation frame based on current game frame. There are optional parameters for customization
void DrawPlayer(Player* player, int frame, int animation_fps = 12, float size_multiplier = 1.0, Color color = WHITE)
{
    if (animation_fps != 0)
    {
        if      ((frame % (GAME_FPS / animation_fps) == 0) && (player->current_frame <  6)) ++player->current_frame;
        else if ((frame % (GAME_FPS / animation_fps) == 0) && (player->current_frame == 6))   player->current_frame = 0;
    }
    
    DrawTexturePro(player->sprite, {player->current_frame * PLAYER_WIDTH, 0, PLAYER_WIDTH, PLAYER_HEIGHT},
                  {player->position_x, player->position_y, PLAYER_WIDTH * size_multiplier, PLAYER_HEIGHT * size_multiplier},
                  {0, 0}, min(14, (int)(player->speed * 1.33)), color);
}

// Starts a single player game with the selected difficulty/gamemode
void StartSingleplayerGame(Difficulty difficulty, int *ranking)
{
    bool replay = true;
    int frame, score;
    

    Player player;
    player.sprite = LoadTexture("./resources/sprites/characters/queroquero.png");
    
    Texture2D background = LoadTexture("./resources/sprites/scenes/DESERT/DESERT.png");
    
    Obstacle obs_list[OBSTACLE_QUANTITY];
    Texture2D obs_texture = LoadTexture("./resources/sprites/pipe.png");
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
    {
        obs_list[i].sprite             = obs_texture;
        obs_list[i].speed              = difficulty.initial_obstacle_speed;
        obs_list[i].gap_height         = difficulty.initial_gap_height;
        obs_list[i].gap_position_delta = difficulty.initial_gap_position_delta;
    }
        
    while (replay)
    {
        replay = false;
        
        frame = 0;
        score = 0;
        
        player.speed         = 0;
        player.jump_speed    = difficulty.initial_player_jump_speed;
        player.acceleration  = difficulty.initial_player_acceleration;
        player.position_x    = difficulty.initial_player_position_x;
        player.position_y    = 300;
        player.current_frame = 0;
        
        int gap_height_aux = difficulty.initial_gap_height / 2;
        for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        {
            if (i > 0) obs_list[i].gap_position_y = GetRandomValue(max(gap_height_aux, (int)obs_list[i-1].gap_position_y - gap_height_aux), min(WINDOW_HEIGHT - gap_height_aux, (int)obs_list[i-1].gap_position_y + gap_height_aux));
            else       obs_list[i].gap_position_y = GetRandomValue(gap_height_aux, WINDOW_HEIGHT - gap_height_aux);
            
            obs_list[i].position_x  = WINDOW_WIDTH  + (OBSTACLE_WIDTH + difficulty.initial_obstacle_distance) * i;
        }

        while ((!WindowShouldClose()) && (!replay))
        {
            ///////////
            ++frame;
            ///////////
            
            UpdateObstacles(obs_list, &score, difficulty);
           
            if (frame > GAME_FPS / 2)
            {
                if (IsKeyPressed(KEY_SPACE)) player.speed = -player.jump_speed;
                else                         player.speed += player.acceleration;
            }
            
            player.position_y += player.speed;
            
            
            BeginDrawing();
                
                DrawTexture(background, 0, 0, WHITE);
                DrawPlayer(&player, frame, 14, 1, {255,0,0,128});
                
                for (Obstacle obs : obs_list)
                    DrawObstacle(obs);
                
                DrawRectangle(0, 40, WINDOW_WIDTH, 50, {0, 0, 0, 128});
                sprintf(str_buffer, "Score: %d Ranking: %d %d %d %d %d", score, ranking[0], ranking[1], ranking[2], ranking[3], ranking[4]);
                DrawText(str_buffer, 50, 50, 30, WHITE);
                
            EndDrawing();
            
            if (CheckPlayerCollision(player, obs_list))
            {
                for (int i = 0; i < RANKING_QUANTITY; ++i)
                {
                    if (score > ranking[i])
                    {
                        for (int j = RANKING_QUANTITY; j > i; --j)
                            ranking[j] = ranking[j-1];
                        
                        ranking[i] = score;
                        
                        sprintf(str_buffer, "Highscore: %d", score);
                        
                        break;
                    }
                }
                
                
                player.speed = 0;
                
                // Plays the bird death animation
                while (player.position_y <= WINDOW_HEIGHT + 100)
                {
                    player.speed      += player.acceleration * 2;
                    player.position_y += player.speed;
                    
                    BeginDrawing();
                    
                        DrawTexture(background, 0, 0, WHITE);
                        DrawPlayer(&player, 0.1, 0);
                        
                        for (Obstacle obs : obs_list)
                            DrawObstacle(obs);
                        
                        DrawRectangle(0, 40, WINDOW_WIDTH, 50, {0, 0, 0, 128});
                        DrawText(str_buffer, 1000, 50, 30, WHITE);
                
                    EndDrawing();
                }
                
                // Shows the replay screen
                while ((!WindowShouldClose()) && (!replay))
                {
                    BeginDrawing();
                        DrawTexture(background, 0, 0, WHITE);
                        DrawPlayer(&player, 0.1, 0);
                                
                        for (Obstacle obs : obs_list)
                            DrawObstacle(obs);
                                
                        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0, 0, 0, 128});
                        DrawText(str_buffer, 475, 350, 40, WHITE);
                        DrawText("Try again?", 450, 400, 40, {255, 255, 255, 255});
                    EndDrawing();
                    
                    if (IsKeyPressed(KEY_ESCAPE)) return;
                    if (IsKeyPressed(KEY_SPACE)) replay = true;
                }
            }
        }
    }
}





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
        
        
        
        if (background_bird.position_x >= WINDOW_WIDTH + 50)
        {
            // It will take the bird aproximately (5, 15) seconds to reach the screen from the new position
            background_bird.position_x = GetRandomValue(5, 15) * background_bird.speed * GAME_FPS * -1;
            background_bird.position_y = GetRandomValue(100, 500);
            size                       = GetRandomValue(2, 4) * 0.1;
        }
        background_bird.position_x += background_bird.speed;
        
        
                
        if ((IsKeyPressed(KEY_RIGHT)) && ((MenuButton)selector != EXITGAME))         ++selector;
        if ((IsKeyPressed(KEY_LEFT))  && ((MenuButton)selector != SINGLEPLAYERGAME)) --selector;
        
        if ((MenuButton)selector == SINGLEPLAYERGAME)
        {
            if ((IsKeyPressed(KEY_UP))    && ((GameDifficulty)difficulty_selector != CHILL))  --difficulty_selector;
            if ((IsKeyPressed(KEY_DOWN))  && ((GameDifficulty)difficulty_selector != CUSTOM)) ++difficulty_selector;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            int ranking[5] = {5,4,3,2,1};
            switch ((MenuButton)selector)
            {
                case SINGLEPLAYERGAME:
                    StartSingleplayerGame(LoadDifficulty(difficulty_selector), ranking);
                    break;

                case EXITGAME:
                    return 0;
                default:
                    // this shouldnt be able to happen
                    break;
            }
        }
        
        
        
        UpdateMusicStream(background_music);
        
        BeginDrawing();
            DrawTexture(background, 0, 0, WHITE);
            DrawPlayer(&background_bird, frame, 12, size);
            sprintf(str_buffer, "Seletor: [%d]", selector);
            DrawText(str_buffer, 600, 400, 30, RED);
            sprintf(str_buffer, "Dificuldade: [%d]", difficulty_selector);
            DrawText(str_buffer, 950, 100, 25, RED);
        EndDrawing();
    }
    
    
    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}






