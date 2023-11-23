#include <winsock2.h>
#include "network_datatypes.c"






// Description: Creates the main server socket and enters the game loop
int main(void)
{
    SOCKET server_socket;
	
    if (CreateServerContext(&server_socket) == 0)
	{
		GameLoop(&server_socket);
		
		// After the game loop stops, turn off the server
		DestroyServerContext(&server_socket);
	}
	else return -1;
}



// Description: General socket setup and initialization
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



// Description: Socket deinitialization
void DestroyServerContext(void)
{
    closesocket(server_socket);
    WSACleanup();    
}

void GameLoop()
{
	Client client_list[5]; 
	
	while (!END)
	{
		ReceiveAndDecodeLoginPacket();
	}
	
	while (!GAMEEND)
	{
		ReceiveAndDecodePlayerInfoPacket();
		UpdatePlayerInfo(&client_list);
		SendPlayerInfoToClients(client_list);
	}
}

void ReceiveAndDecodeLoginPacket()
{
	sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);
	
	LoginPacket packet;
	
	recvfrom(server_socket, &packet, sizeof(packet), 0, (sockaddr*)&client_addr, &client_addr_size);
    MessageHandler(packet);
}



void ReceivePacket(SOCKET* server_socket)
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