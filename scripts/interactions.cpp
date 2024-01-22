#include "player.cpp"
#include "obstacle.cpp"


// Checks if the player collided with any obstacle or got out of the screen.
bool CheckPlayerCollision(Player player, Obstacle* obs_list)
{
    // The player rectangle is a bit smaller than the actual sprite to take into consideration its rotation, avoiding some strange collisions.
    Rectangle2D player_rect = {player.position_x + 5, player.position_y + 5, PLAYER_WIDTH - 10, PLAYER_HEIGHT - 10};
    
    // Checks if the player is out of the screen (with some tolerance to allow for some risky jumps).
    if ((player.position_y >= WINDOW_HEIGHT + 30) || (player.position_y <= -130)) 
        return true;
    
    // Checks if the player has collided with any of the obstacles.
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        if ((CheckCollisionRecs(player_rect, {obs_list[i].position_x, obs_list[i].gap_position_y + (obs_list[i].gap_height / 2),                   OBSTACLE_WIDTH, OBSTACLE_HEIGHT})) ||
            (CheckCollisionRecs(player_rect, {obs_list[i].position_x, obs_list[i].gap_position_y - (obs_list[i].gap_height / 2) - OBSTACLE_HEIGHT, OBSTACLE_WIDTH, OBSTACLE_HEIGHT})))
            return true;
    
    // If neither test-cases before returned true, the player hasn't collided.
    return false;
}