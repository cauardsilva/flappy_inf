#include <winsock2.h>
#include <stdio.h>
#include <pthread.h>

enum MessageID {
    CLIENT_CONNECTION = 0,
    PLAYER_INFORMATION = 1
}

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

typedef struct ClientPacket
{
    MessageID msgType;
    byte ID;
} ClientPacket;


// Description: Creates the main server socket and enters the client message loop
int main(void)
{
    SOCKET server_socket;
    if (CreateServerContext(&server_socket) != 0) return -1;

    CreateGameContext();
    UpdateGameContext();



    DestroyGameContext();
    DestroyServerContext();
}

int CreateServerContext(SOCKET* server_socket)
{
    // Loads the required DLLs for WinSock v2.2 usage
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    // Sets up socket address (localhost) and port
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(43424);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_socket* = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));

    return 0;
}



void DestroyServerContext(void)
{
    closesocket(server_socket);
    WSACleanup();    
}



void MessageLoop(SOCKET* server_socket)
{
    sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    ClientPacket packet;

    while (true)
    {
        recvfrom(server_socket, &packet, sizeof(packet), 0, (sockaddr*)&client_addr, &client_addr_size);
        MessageHandler(packet);
    }   
}

void MessageHandler(ClientPacket packet)
{
    switch (packet.msgType)
    {
        case CLIENT_CONNECTION:
            /* code */
            break;

        case PLAYER_INFORMATION:
            /* code */
            break;
    
        default: // Unknown msgType
            break;
    }
}