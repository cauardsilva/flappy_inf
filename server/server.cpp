#include <iostream>
#include "../sockpp/include/sockpp/udp_socket.h"
#include <time.h>
#include <unistd.h>


typedef struct ServerPlayer 
{
    float speed, acceleration, position_y;
} ServerPlayer;

typedef struct ServerPlayerPacket 
{
    float speed, position_y;
} ServerPlayerPacket;


typedef struct ServerObstacle
{
    float speed, acceleration, position_x, gap_height, gap_position_y;
} ServerObstacle;

typedef struct ServerObstaclePacket
{
    float position_x, gap_height, gap_position_y;   
} ServerObstaclePacket;

using namespace std;



void serializeobs(char* serialized, ServerObstacle obs)
{
    serialized[0] = (int)obs.position_x/1000;
    serialized[1] = (int)(obs.position_x-serialized[0])/100;
    serialized[2] = (int)(obs.position_x-serialized[1]-serialized[0])/10;
    serialized[3] = (int)obs.position_x%10;
    
    serialized[4] = (int)obs.gap_height/100;
    serialized[5] = (int)(obs.gap_height-serialized[4])/10;
    serialized[6] = (int)obs.gap_height%10;
    
    serialized[7] = (int)obs.gap_position_y/100;
    serialized[8] = (int)(obs.gap_position_y-serialized[7])/10;
    serialized[9] = (int)obs.gap_position_y%10;
}



int main() {
    srand(time(NULL));
    




   
    
    ServerObstacle obs;
    obs.position_x     = 1300;
    obs.gap_height     = 350;
    obs.gap_position_y = 400;
    
    char serializedobs[10]; 
    
    while (true)
    {
        obs.position_x -= 10;
        if (obs.position_x <= -100)
        {
            obs.position_x     = 1300;
            obs.gap_position_y = (rand() % 500) + 150;
        }
        serializeobs(serializedobs, obs);
        
        
        
        sleep(1/60);
    }
    
    
    
    
    
    



    return 0;
}