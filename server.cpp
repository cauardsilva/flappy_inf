#include <winsock2.h>
#include "raylib.h"
#include <string.h>
#include <iostream>
#include <fstream>

#include "./scripts/interactions.cpp"


#define CLIENT_QUANTITY 2

typedef struct Client {
    sockaddr_in address;
    Player player;
} Client;



using namespace std;

int main()
{
 
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    
    SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in server_address;
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port        = htons(43424);
    
    bind(server_socket, (sockaddr*)&server_address, sizeof(server_address));
    
    sockaddr_in client_address;
    int client_address_size = sizeof(client_address);
    
    char buffer;
    
    int connected_clients = 0;
    Client client_list[CLIENT_QUANTITY];
    
    bool found = false;
    
    while (connected_clients != CLIENT_QUANTITY)
    {
        
        recvfrom(server_socket, &buffer, sizeof(char), 0, (sockaddr*)&client_address, &client_address_size);

        found = false;
        
        for (Client client : client_list)
        {
            if (client.address.sin_addr.s_addr == client_address.sin_addr.s_addr)
                found = true;
        }
        
        if (!found)
        {
            client_list[connected_clients].address              = client_address;
            client_list[connected_clients].player.current_frame = 0;
            client_list[connected_clients].player.speed         = 6;
            client_list[connected_clients].player.jump_speed    = 1;
            client_list[connected_clients].player.acceleration  = 0;
            client_list[connected_clients].player.position_x    = 96;
            client_list[connected_clients].player.position_y    = 200;
            
                
            ++connected_clients;
            cout << "New client! IP: " << inet_ntoa(client_address.sin_addr);
        }
    }
    cout << "All clients connected!";
    while (true)
    {
        

        for (int i = 0; i < CLIENT_QUANTITY; ++i)
        {
            UpdatePlayer(&(client_list[i].player));
            if ((client_list[i].address.sin_addr.s_addr == client_address.sin_addr.s_addr) && (buffer == 1))
            {
                if (client_list[i].player.speed == 0)
                    client_list[i].player.speed = -client_list[i].player.jump_speed;
                else
                    client_list[i].player.speed = -client_list[i].player.speed;
        
            }
        }
        
        for (Client client : client_list)
        {
            sendto(server_socket, (char*)client_list, sizeof(Client)*CLIENT_QUANTITY, 0, (sockaddr*)&client.address, sizeof(client.address));
            cout << "jpos: " << client.player.position_y << endl;
        }

        recvfrom(server_socket, &buffer, sizeof(buffer), 0, (sockaddr*)&client_address, &client_address_size);

    }
    
    
    
    WSACleanup();
    
    
    
    return 0;
}