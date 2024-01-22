#include "raylib.h"
#include "difficulty.cpp"




#define OBSTACLE_WIDTH  200
#define OBSTACLE_HEIGHT 800

#define OBSTACLE_QUANTITY (WINDOW_WIDTH / OBSTACLE_WIDTH) + 1


using namespace std;

typedef struct Obstacle
{
    Texture2D sprite;
    float speed, acceleration, position_x, gap_height, gap_position_y, gap_position_delta;
} Obstacle;


// Draws both (upper and lower) portions of an obstacle.
void DrawObstacle(Obstacle obstacle)
{
    DrawTexturePro(obstacle.sprite, {0, 0, OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {obstacle.position_x, obstacle.gap_position_y + (obstacle.gap_height / 2), OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {0, 0}, 0, WHITE);
    DrawTexturePro(obstacle.sprite, {0, 0, OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {obstacle.position_x + OBSTACLE_WIDTH, obstacle.gap_position_y - (obstacle.gap_height / 2), OBSTACLE_WIDTH, OBSTACLE_HEIGHT}, {0, 0}, 180, WHITE);
}


// Updates all obstacles based on game data (if provided), such as score and difficulty
void UpdateObstacles(Obstacle *obs_list, int *score, Difficulty difficulty)
{
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
    {
        // Updates the obstacle variables based on the current score.
        float current_obstacle_speed     = difficulty.initial_obstacle_speed     + (difficulty.obstacle_speed_increment     * (int)(*score / difficulty.score_threshold));
        float current_gap_height         = difficulty.initial_gap_height         - (difficulty.gap_height_increment         * (int)(*score / difficulty.score_threshold)) > (PLAYER_HEIGHT * 2) ? difficulty.initial_gap_height         - (difficulty.gap_height_increment         * (int)(*score / difficulty.score_threshold)) :  (PLAYER_HEIGHT * 2);
        float current_gap_position_delta = difficulty.initial_gap_position_delta + (difficulty.gap_position_delta_increment * (int)(*score / difficulty.score_threshold));
        
        obs_list[i].speed              = current_obstacle_speed;
        obs_list[i].gap_height         = current_gap_height;
        obs_list[i].gap_position_delta = current_gap_position_delta;
        
        
        if (obs_list[i].position_x >= -OBSTACLE_WIDTH)
            obs_list[i].position_x -= obs_list[i].speed; 
        else 
        {
            // Checks if the new distance (based on score) is above zero. If it is lesser than zero, sets it to zero (we don't want any obstacles overlapping each other).
            float current_obstacle_distance = difficulty.initial_obstacle_distance - (difficulty.obstacle_distance_increment * (int)(*score / difficulty.score_threshold)) > 0 ? difficulty.initial_obstacle_distance - (difficulty.obstacle_distance_increment * (int)(*score / difficulty.score_threshold)) : 0;
            
            obs_list[i].position_x = current_obstacle_distance + (OBSTACLE_WIDTH + current_obstacle_distance) * OBSTACLE_QUANTITY;
            
            
            int gap_delta_aux  = obs_list[i].gap_position_delta / 2;
            int gap_height_aux = obs_list[i].gap_height         / 2;
            
            if (i > 0)
                obs_list[i].gap_position_y = GetRandomValue(max(gap_height_aux, (int)(obs_list[i-1].gap_position_y - gap_delta_aux)), min(WINDOW_HEIGHT - gap_height_aux, (int)(obs_list[i-1].gap_position_y + gap_delta_aux)));
            else
                obs_list[i].gap_position_y = GetRandomValue(max(gap_height_aux, (int)(obs_list[OBSTACLE_QUANTITY-1].gap_position_y - gap_delta_aux)), min(WINDOW_HEIGHT - gap_height_aux, (int)(obs_list[OBSTACLE_QUANTITY-1].gap_position_y + gap_delta_aux)));

            
            *score += 1;
        }
    }
}