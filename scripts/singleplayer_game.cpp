#include <ctime>



void StartSingleplayerGame()
{    
    Rank ranking[RANKING_QUANTITY];
    LoadRanking(ranking);

    
    ParallaxBackground parallax_background;
    for (int i = 0; i < 4; ++i)
    {
        parallax_background.layers[i]           = LoadTexture(TextFormat("./resources/sprites/scenes/%d.png", i));
        parallax_background.layers_positions[i] = 0;
    }
    parallax_background.default_speed = .5;
    
    Color background_color;


    Player player;
    player.sprite          = LoadTexture("./resources/sprites/characters/queroquero.png");
    player.size_multiplier = 1.0;
    player.position_x      = 96;
    player.position_y      = 300;
    player.color           = WHITE;
    
    Sound death_sound = LoadSound("./resources/sounds/player_death.wav");
    
    Font font = LoadFont("./resources/fonts/queroquero.ttf");
    
    Obstacle obs_list[OBSTACLE_QUANTITY];
    
    Texture obs_texture = LoadTexture("./resources/sprites/obstacles/pipe.png");
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        obs_list[i].sprite = obs_texture;
    
    

    Music background_music = LoadMusicStream("./resources/sounds/wind_background.mp3");
    SetMusicVolume(background_music, .05);

    PlayMusicStream(background_music);
    
    
    time_t delay = time(0);
    
    int frame = 0;
    int difficulty_selector = 1;
    
    while ((!WindowShouldClose()) && (!((IsKeyPressed(KEY_SPACE)) && (time(0) - delay >= 1))))
    {
        ++frame;
        
       
        UpdateMusicStream(background_music);
        
        background_color = DifficultyColor[difficulty_selector];
        
        BeginDrawing();
        
                DrawParallaxBackground(&parallax_background, 4, background_color);
                DrawPlayer(player, frame);
                DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0,0,0,128});
                DrawTextEx(font, "DIFFICULTY", {96, 635}, 32, 0, WHITE);
                DrawTextEx(font, DifficultyText[difficulty_selector], {96, 659}, 64, 0, DifficultyColor[difficulty_selector]);
                
        EndDrawing();
        
        if ((IsKeyPressed(KEY_UP))   && ((GameDifficulty)difficulty_selector != LAST))  ++difficulty_selector;
        if ((IsKeyPressed(KEY_DOWN)) && ((GameDifficulty)difficulty_selector != FIRST)) --difficulty_selector;
        
    }

    
    Difficulty initial_difficulty = LoadDifficulty(difficulty_selector);
    Difficulty difficulty;
    
    
    
    bool replay = true;
    int score;
    
    while (replay)
    {
        
        replay = false;
        
        frame = 0;
        score = 0;
        
        
        parallax_background.default_speed = .5;
        
        difficulty = initial_difficulty;
        
        InitializeObstacles(obs_list, difficulty);
        InitializePlayer(&player, difficulty);
        
        
        while ((!WindowShouldClose()) && (!replay))
        {
            ++frame;
            
       
            UpdateMusicStream(background_music);
            

            UpdateObstacles(obs_list, &difficulty, &score);
                
            if (IsKeyPressed(KEY_SPACE))
                player.speed = -player.jump_speed;
            
            UpdatePlayer(&player);


            
            BeginDrawing();
                DrawParallaxBackground(&parallax_background, 4, background_color);
                
                DrawPlayer(player, frame);
                
                for (Obstacle obs : obs_list)
                    DrawObstacle(obs);
                
                DrawRectangle(0, 40, WINDOW_WIDTH, 50, {0, 0, 0, 128});
                DrawText(TextFormat("Score: %d", score), 50, 50, 30, WHITE);
                DrawText(TextFormat("Top score: %d", ranking[0].score > score ? ranking[0].score : score), WINDOW_WIDTH - 250, 50, 30, WHITE);
                
            EndDrawing();
            
            
            if (CheckPlayerCollision(player, obs_list))
            {
                player.speed   = 0;
                player.gravity = .8 * copysign(1, player.gravity);                  
                
                parallax_background.default_speed = 0;
                
                PlaySound(death_sound);
                
                // Plays the quero-quero death animation
                while ((player.position_y <= WINDOW_HEIGHT + 80) && (player.position_y >= -350))
                {
                    UpdatePlayer(&player);
                    
                    BeginDrawing();
                    
                        DrawParallaxBackground(&parallax_background, 4, background_color);
                        DrawPlayer(player, 0.1, 0);
                        
                        for (Obstacle obs : obs_list)
                            DrawObstacle(obs);
                        
                    EndDrawing();
                }
                
                // If the player has achieved a highscore, updates the ranking.
                for (int i = 0; i < RANKING_QUANTITY; ++i)
                {
                    if (score > ranking[i].score)
                    {
                        char highscore_name_buffer[RANKING_NAME_LENGTH] = "";
                        
                        int frame_aux = 0;
                        Color color_aux;
                        
                        int input_length = 0, key_pressed = 0;
                        
                        while ((!IsKeyPressed(KEY_ESCAPE)) && (!((IsKeyPressed(KEY_ENTER)) && (highscore_name_buffer[0] != '\0'))) && (!WindowShouldClose()))
                        {
                            ++frame_aux;
                            
                            if (frame_aux % 6 == 0)
                                color_aux = {GetRandomValue(127, 255), GetRandomValue(127, 255), GetRandomValue(127, 255), 255};
                            
                            
                            BeginDrawing();
          
                                DrawParallaxBackground(&parallax_background, 4, background_color);
                                        
                                for (Obstacle obs : obs_list)
                                    DrawObstacle(obs);
                                        
                                DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0, 0, 0, 128});
                                
                                DrawText(TextFormat("NEW HIGHSCORE! %d", score), (WINDOW_WIDTH/2) - (MeasureText(TextFormat("NEW HIGHSCORE! %d", score), 40)/2), 350, 40, color_aux);
                                
                                DrawText("Enter with your nickname:", (WINDOW_WIDTH/2) - (MeasureText("Enter with your nickname:", 30)/2), 400, 30, WHITE);
                                
                                DrawRectangle((WINDOW_WIDTH/2) - (MeasureText(highscore_name_buffer, 30)/2) - 10, 440, MeasureText(highscore_name_buffer, 30) + 20, 50, {0,0,0,64});
                                DrawText(highscore_name_buffer, (WINDOW_WIDTH/2) - (MeasureText(highscore_name_buffer, 30)/2), 450, 30, WHITE);
                  
                            EndDrawing();
                            
                            key_pressed = GetKeyPressed();
                                
                             
                            if ((((IsKeyPressed(KEY_SPACE)) && (input_length != 0)) || (key_pressed >= 65 && key_pressed <= 90) || (key_pressed >= 97 && key_pressed <= 122)) && (input_length < RANKING_NAME_LENGTH-1))
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
                        }


                        for (int j = RANKING_QUANTITY-1; j > i; --j)
                            ranking[j] = ranking[j-1];
                        
                        strcpy(ranking[i].name, highscore_name_buffer);
                        ranking[i].score = score;
                        
                        
                        SaveRanking(ranking);
                        
                        break;
                    }
                }
                
                
                // Shows the replay screen
                while ((!WindowShouldClose()) && (!replay))
                {
                    BeginDrawing();
                    
                        DrawParallaxBackground(&parallax_background, 4, background_color);
                                
                        for (Obstacle obs : obs_list)
                            DrawObstacle(obs);
                                
                        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0, 0, 0, 128});
                        DrawText(TextFormat("Score: %d", score), WINDOW_WIDTH/2 - MeasureText(TextFormat("Score: %d", score), 40)/2, 350, 40, WHITE);
                        DrawText("Try again?", WINDOW_WIDTH/2 - MeasureText("Try again?", 40)/2, 400, 40, WHITE);
                        
                    EndDrawing();
                    
                    if (IsKeyPressed(KEY_ESCAPE)) return;
                    if (IsKeyPressed(KEY_SPACE))  replay = true;
                }
            }
        }
    }
}







