#define MAX_CLIENTS 8
#define MAX_WAIT_USEC 100000
#define MAX_WAIT_SINGLE_CLIENT_MSEC 100

enum PlayerAction {NONE=0, JUMP=1};



typedef struct InitialPacket {
    int client_quantity, client_index;
} InitialPacket;



typedef struct PlayerInfo {
	float speed, position_x, position_y;
	Color color;
} PlayerInfo;



typedef struct ObstacleInfo {
	float position_x, gap_height, gap_position_y;
} ObstacleInfo;



typedef struct Packet {
    PlayerInfo player_list[MAX_CLIENTS];
    ObstacleInfo obs_list[OBSTACLE_QUANTITY];
} Packet;
