#include <winsock2.h>
#include "raylib.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>       


#include <unistd.h>

#include "./scripts/interactions.cpp"




typedef struct Client {
    sockaddr_in address;
    Player player;
    Color color;
} Client;


enum PlayerAction {NONE=0, JUMP=1};


using namespace std;

int main()
{
    int qty;
    cout << "Type the number of clients: ";
    cin >> qty;
    
    const char CLIENT_QUANTITY = qty;
    
    srand(time(NULL));
    
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
    
    
    
    char buffer = 0;
    
    Client client_list[CLIENT_QUANTITY];
    char total_buffer[CLIENT_QUANTITY];
    bool already_connected = false;
    
    for (int i = 0; i < (int)CLIENT_QUANTITY; ++i)
    {
        recvfrom(server_socket, &buffer, sizeof(char), 0, (sockaddr*)&client_address, &client_address_size);

        already_connected = false;
        
        for (Client client : client_list)
        {
            if (client.address.sin_addr.s_addr == client_address.sin_addr.s_addr)
            {
                already_connected = true;
                break;
            }
        }
        
        if (!already_connected)
        {
            client_list[i].address              = client_address;
            client_list[i].player.current_frame = 0;
            client_list[i].player.speed         = 0;
            client_list[i].player.jump_speed    = 10;
            client_list[i].player.acceleration  = .3;
            client_list[i].player.position_x    = 96;
            client_list[i].player.position_y    = 200;
            client_list[i].color                = {rand() % 256, rand() % 256, rand() % 256, 255};
            
            cout << "New client connected! IP: " << inet_ntoa(client_address.sin_addr) << endl;
            
            sendto(server_socket, &CLIENT_QUANTITY, sizeof(char), 0, (sockaddr*)&client_address, sizeof(client_address));
        }
    }
    
    
    cout << "All clients connected! Starting game.";
    
    
    time_t initial_time;
 
    while (true)
    {
        for (int i = 0; i < CLIENT_QUANTITY; ++i)
        {
            UpdatePlayer(&(client_list[i].player));
            
            if ((PlayerAction)total_buffer[i] == JUMP)
                client_list[i].player.speed = -client_list[i].player.jump_speed;
        }
        
        for (Client client : client_list)
            sendto(server_socket, (char*)client_list, sizeof(Client)*CLIENT_QUANTITY, 0, (sockaddr*)&client.address, sizeof(client.address));
        
        for (int i = 0; i < CLIENT_QUANTITY; ++i)
        {
            initial_time = time(0);

            recvfrom(server_socket, &buffer, sizeof(char), 0, (sockaddr*)&client_address, &client_address_size);
            for (int j = 0; j < CLIENT_QUANTITY; ++j)
            {
                if (client_list[i].address.sin_addr.s_addr == client_address.sin_addr.s_addr)
                    total_buffer[i] = buffer;
            }
        }

        
        usleep(1000000/480); // Receives/sends approximately 480 packets per second
    }
    
    
    
    WSACleanup();
    
    
    
    return 0;
}