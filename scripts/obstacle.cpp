typedef struct Obstacle
{
    Texture2D sprite;
    float position_x, gap_height, gap_position_y, gap_position_delta;
} Obstacle;


// Draws both (upper and lower) portions of an obstacle.
void DrawObstacle(Obstacle obstacle)
{
    DrawTexturePro(obstacle.sprite, {0, 0, OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {obstacle.position_x,                  obstacle.gap_position_y + (obstacle.gap_height / 2), OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {0, 0}, 0, WHITE);
    DrawTexturePro(obstacle.sprite, {0, 0, OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {obstacle.position_x + OBSTACLE_WIDTH, obstacle.gap_position_y - (obstacle.gap_height / 2), OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {0, 0}, 180, WHITE);
}



void InitializeObstacles(Obstacle *obs_list, Difficulty difficulty)
{
    float gap_delta_aux  = difficulty.gap_position_delta / 2;
    float gap_height_aux = difficulty.gap_height         / 2;
        
    // Initializes the obstacles
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
    {   
         obs_list[i].gap_height         = difficulty.gap_height;
         obs_list[i].gap_position_delta = difficulty.gap_position_delta;
         obs_list[i].position_x         = WINDOW_WIDTH + (OBSTACLE_WIDTH + difficulty.obstacle_distance) * i;
         
         if (i > 0) obs_list[i].gap_position_y = GetRandomValue((int)(obs_list[i-1].gap_position_y - gap_delta_aux), (int)(obs_list[i-1].gap_position_y + gap_delta_aux));
         else       obs_list[i].gap_position_y = GetRandomValue(gap_height_aux, WINDOW_HEIGHT - gap_height_aux);
         
         // Limits the gap position to fit inside the upper and lower portions of the screen         
         obs_list[i].gap_position_y = FitToRange(obs_list[i].gap_position_y, gap_height_aux, WINDOW_HEIGHT - gap_height_aux);
     }
}


// Updates all obstacles based on game data (if provided), such as score and difficulty
void UpdateObstacles(Obstacle *obs_list, Difficulty *difficulty, int *score = NULL)
{
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        obs_list[i].position_x -= difficulty->obstacle_speed; 
    
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
    {
        if (obs_list[i].position_x <= -OBSTACLE_WIDTH)
        {
            if (score != NULL)
            {
                *score += 1;
                
                if ((difficulty->score_threshold != 0) && (fmod(*score, difficulty->score_threshold) == 0))
                    UpdateDifficulty(difficulty);
            }

            obs_list[i].position_x = obs_list[i].position_x + OBSTACLE_WIDTH + difficulty->obstacle_distance + ((OBSTACLE_WIDTH + difficulty->obstacle_distance) * (OBSTACLE_QUANTITY - 1));
            
            obs_list[i].gap_height         = difficulty->gap_height;
            obs_list[i].gap_position_delta = difficulty->gap_position_delta;
            
            
            float gap_delta_aux  = obs_list[i].gap_position_delta / 2;
            float gap_height_aux = obs_list[i].gap_height         / 2;
            
            
            if (i > 0) obs_list[i].gap_position_y = GetRandomValue((int)(obs_list[i-1].gap_position_y - gap_delta_aux), (int)(obs_list[i-1].gap_position_y + gap_delta_aux));
            else       obs_list[i].gap_position_y = GetRandomValue((int)(obs_list[OBSTACLE_QUANTITY-1].gap_position_y - gap_delta_aux), (int)(obs_list[OBSTACLE_QUANTITY-1].gap_position_y + gap_delta_aux));
            
            // Limits the gap position to fit inside the upper and lower portions of the screen
            obs_list[i].gap_position_y = FitToRange(obs_list[i].gap_position_y, gap_height_aux, WINDOW_HEIGHT - gap_height_aux);
        }
    }
}