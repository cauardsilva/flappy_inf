typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

typedef struct ClientPacket
{
    MessageID msgType;
    byte ID;
} ClientPacket;

typedef struct Client
{
	
} Client;

enum MessageID {
    CLIENT_CONNECTION = 0,
    PLAYER_INFORMATION = 1
}