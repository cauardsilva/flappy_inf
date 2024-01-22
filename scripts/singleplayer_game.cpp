#include "ranking.cpp"
#include <ctime>

using namespace std;


// Starts a single player game with the selected difficulty/gamemode
void StartSingleplayerGame(Difficulty difficulty, Rank *ranking)
{
    bool replay = true;
    int frame, score;
    

    Player player;
    player.sprite = LoadTexture("./resources/sprites/characters/queroquero.png");
    
    Texture2D background = LoadTexture("./resources/sprites/scenes/DESERT/DESERT.png");

    
    
    Obstacle obs_list[OBSTACLE_QUANTITY];
    Texture2D obs_texture = LoadTexture("./resources/sprites/obstacles/pipe.png");
    
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        obs_list[i].sprite = obs_texture;
        

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
        

        int gap_delta_aux  = difficulty.initial_gap_position_delta / 2;
        int gap_height_aux = difficulty.initial_gap_height         / 2;
        
        // Initializes the obstacles
        for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        {
            if (i > 0)
                obs_list[i].gap_position_y = GetRandomValue(max(gap_height_aux, (int)(obs_list[i-1].gap_position_y - gap_delta_aux)), min(WINDOW_HEIGHT - gap_height_aux, (int)(obs_list[i-1].gap_position_y + gap_delta_aux)));
            else       
                obs_list[i].gap_position_y = GetRandomValue(gap_height_aux, WINDOW_HEIGHT - gap_height_aux);
            
            obs_list[i].speed              = difficulty.initial_obstacle_speed;
            obs_list[i].gap_height         = difficulty.initial_gap_height;
            obs_list[i].gap_position_delta = difficulty.initial_gap_position_delta;
            
            obs_list[i].position_x = WINDOW_WIDTH + (OBSTACLE_WIDTH + difficulty.initial_obstacle_distance) * i;
        }
        time_t initial_time = time(0), end_time;
        while ((!WindowShouldClose()) && (!replay))
        {
            ///////////
            ++frame;
            ///////////
            
            if (frame > (GAME_FPS / 2))
            {
                UpdateObstacles(obs_list, &score, difficulty);
                
                if (IsKeyPressed(KEY_SPACE))
                    player.speed = -player.jump_speed;
            
                UpdatePlayer(&player);
            }

            
            BeginDrawing();
                DrawTexture(background, 0, 0, WHITE);
                DrawPlayer(&player, frame, 14, 1, WHITE);
                
                for (Obstacle obs : obs_list)
                    DrawObstacle(obs);
                
                DrawRectangle(0, 40, WINDOW_WIDTH, 50, {0, 0, 0, 128});
                DrawText(TextFormat("Score: %d", score), 50, 50, 30, WHITE);
                DrawText(TextFormat("Top score: %d", ranking[0].score), WINDOW_WIDTH - 250, 50, 30, WHITE);
                
            EndDrawing();
            
            if (CheckPlayerCollision(player, obs_list))
            {
                end_time = time(0);
                // If the player has achieved a highscore, updates the ranking.
                for (int i = 0; i < RANKING_QUANTITY; ++i)
                {
                    if (score > ranking[i].score)
                    {
                        
                        char highscore_name_buffer[17] = "";
                        
             
                        int frame_aux = 0;
                        Color color_aux = WHITE;
                        int input_length = 0, key_pressed = 0;
                        while ((!IsKeyPressed(KEY_ENTER)) && (!IsKeyPressed(KEY_ESCAPE)) && (!WindowShouldClose()))
                        {
                            ++frame_aux;
                            
                            if (frame_aux % 6 == 0)
                                color_aux = {GetRandomValue(127, 255), GetRandomValue(127, 255), GetRandomValue(127, 255), 255};
                            
                            
                            BeginDrawing();
          
                                DrawTexture(background, 0, 0, WHITE);
                                DrawPlayer(&player, 0.1, 0);
                                        
                                for (Obstacle obs : obs_list)
                                    DrawObstacle(obs);
                                        
                                DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0, 0, 0, 128});
                                
                                DrawText(TextFormat("NEW HIGHSCORE! %d", score), 390, 350, 40, color_aux);
                                DrawText("Enter with your nickname:", 390, 400, 30, WHITE);
                                DrawText(highscore_name_buffer, 390, 450, 30, WHITE);
                  
                                key_pressed = GetKeyPressed();
                                
                                if (((key_pressed == 32) || (key_pressed >= 65 && key_pressed <= 90) || (key_pressed >= 97 && key_pressed <= 122)) && (input_length < 16))
                                {
                                    highscore_name_buffer[input_length]   = key_pressed;
                                    highscore_name_buffer[input_length+1] = '\0';
                                    ++input_length;
                                }
                                
                                if (IsKeyPressed(KEY_BACKSPACE) && (input_length != 0))
                                {
                                    highscore_name_buffer[input_length-1] = '\0';
                                    --input_length;
                                }
        
       
                            EndDrawing();
                        }


                        for (int j = RANKING_QUANTITY; j > i; --j)
                            ranking[j] = ranking[j-1];
                        
                        strcpy(ranking[i].name, highscore_name_buffer);
                        ranking[i].score = score;
                        
                        
                        SaveRanking(ranking);
                        
                        break;
                    }
                }
                
                
                player.speed = 0;
                
                // Plays the quero-quero death animation
                while ((player.position_y <= WINDOW_HEIGHT + 80) && (player.position_y >= -350))
                {
                    player.speed      += player.acceleration * 2;
                    player.position_y += player.speed;
                    
                    BeginDrawing();
                        DrawTexture(background, 0, 0, WHITE);
                        DrawPlayer(&player, 0.1, 0);
                        
                        for (Obstacle obs : obs_list)
                            DrawObstacle(obs);
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
                        DrawText(TextFormat("Score: %d", score), 510, 350, 40, WHITE);
                        DrawText("Try again?", 485, 400, 40, {255, 255, 255, 255});
                        DrawText(TextFormat("TIME: %d", end_time-initial_time), 485, 500, 40, {255, 255, 255, 255});
                    EndDrawing();
                    
                    if (IsKeyPressed(KEY_ESCAPE))
                        return;
                    if (IsKeyPressed(KEY_SPACE)) 
                        replay = true;
                }
            }
        }
    }
}