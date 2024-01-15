typedef struct ServerPlayer 
{
    
    float speed, acceleration, position_x, position_y;
} Player;



typedef struct ServerObstacle
{
    
    float speed, acceleration, position_x, gap_height, gap_position_y;
} Obstacle;