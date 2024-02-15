


#include <cstddef> // byte
#include <winsock2.h>
#include "raylib.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>    
#include <math.h>   
#include <chrono>
using namespace std;



#include <unistd.h>

#include "./scripts/global_constants.cpp"
#include "./scripts/utils.cpp"
#include "./scripts/difficulty.cpp"
#include "./scripts/player.cpp"
#include "./scripts/obstacle.cpp"
#include "./scripts/interactions.cpp"
#include "./scripts/multiplayer_datatypes.cpp"



typedef struct Client {
    sockaddr_in address;
    Player player;
    bool is_alive;
} Client;










int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    
    srand(time(NULL));
    SetRandomSeed(time(NULL));
    
    
    int qty;
    cout << "Type the number of clients: ";
    cin >> qty;
    
    const char CLIENT_QUANTITY = qty;
    
    
    
    SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in server_address;
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port        = htons(43424);
    
    bind(server_socket, (sockaddr*)&server_address, sizeof(server_address));
    
    
    
    // Disabling connection reset error: https://stackoverflow.com/questions/34242622/windows-udp-sockets-recvfrom-fails-with-error-10054
    bool error_enabled = false;
    DWORD output = 0;
    WSAIoctl(server_socket, _WSAIOW(IOC_VENDOR, 12) /*SIO_UDP_CONNRESET*/, &error_enabled, sizeof(error_enabled), NULL, 0, &output, NULL, NULL);
    
    
    
    cout << endl << "Server created with success. Awaiting for new connections..." << endl;
    
    
    
    Client client_list[CLIENT_QUANTITY];
    
    sockaddr_in client_address;
    int client_address_size = sizeof(client_address);
    
    // This buffer is responsible for receiving client data, which is currently a char (unsigned int) representing the action it last took in the game.
    char buffer = 0;
    // This packet stores all necessary information about the game, such as player and obstacle data, and is constantly sent to the clients.
    Packet game_state;
    
    
	
    InitialPacket info;
    bool already_connected = false;
    int current_index = 0;
    
    while (current_index != CLIENT_QUANTITY)
    {
        recvfrom(server_socket, &buffer, sizeof(char), 0, (sockaddr*)&client_address, &client_address_size);

        already_connected = false;
        
        // Check if the client has already connected, as to not add it again to the array
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
            client_list[current_index].address      = client_address;
            client_list[current_index].player.color = game_state.player_list[current_index].color = {rand() % 256, rand() % 256, rand() % 256, 255};
            
            cout << endl << "New client connected! IP: " << inet_ntoa(client_address.sin_addr);
			
            info.client_index    = current_index;
            info.client_quantity = CLIENT_QUANTITY;
            
            // Sends the number of players to each client that connects so it can create its local array
            sendto(server_socket, (char*)&info, sizeof(InitialPacket), 0, (sockaddr*)&client_address, sizeof(client_address));
            
            ++current_index;
        }
    }
    
    
    
    cout << endl << endl << "All clients connected! Starting game.";
    
	
    
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = MAX_WAIT_USEC;
    
    fd_set server_socket_read;
    
    
    
    
    Obstacle obs_list[OBSTACLE_QUANTITY];
    
    PlayerAction action_list[CLIENT_QUANTITY];
    
	Difficulty original_difficulty = LoadDifficulty(SERVER);
    Difficulty difficulty;
	
    time_t start_time;
    unsigned int recv_timer;
    int num_client_action;
    
    int score;
    bool is_everyone_dead;
        
    while (true)
    {
        score = 0;
        is_everyone_dead = false;
        
        difficulty = original_difficulty;
        
        
        for (int i = 0; i < CLIENT_QUANTITY; ++i)
        {
            InitializePlayer(&(client_list[i].player), difficulty);
            client_list[i].is_alive = true;
        }
        
        InitializeObstacles(obs_list, difficulty);
        

        
        while (!is_everyone_dead)
        {
                for (int i = 0; i < CLIENT_QUANTITY; ++i)
                {
                    if (client_list[i].is_alive && !CheckPlayerCollision(client_list[i].player, obs_list))
                    {
                        UpdatePlayer(&(client_list[i].player));
                        
                        if (action_list[i] == JUMP)
                        {
                            client_list[i].player.speed = -client_list[i].player.jump_speed;
                            action_list[i] = NONE;
                        }
                    }
                    else if (!client_list[i].is_alive)
                    {
                        if (client_list[i].player.position_y <= WINDOW_HEIGHT + 80 && client_list[i].player.position_y >= -350)
                            UpdatePlayer(&(client_list[i].player)); 
                    }
                    else 
                    {
                        client_list[i].is_alive       = false;
                        client_list[i].player.speed   = 0;
                        client_list[i].player.gravity = .8 * copysign(1, client_list[i].player.gravity);       
                    }
                    
                    game_state.player_list[i].speed      = client_list[i].player.speed;
                    game_state.player_list[i].position_x = client_list[i].player.position_x;
                    game_state.player_list[i].position_y = client_list[i].player.position_y;
                }
                
                
                
                UpdateObstacles(obs_list, &difficulty, &score);
                
                for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
                {
                    game_state.obs_list[i].position_x     = obs_list[i].position_x;
                    game_state.obs_list[i].gap_position_y = obs_list[i].gap_position_y;    
                    game_state.obs_list[i].gap_height     = obs_list[i].gap_height;
                }
            
            
            
            for (Client client : client_list)
                sendto(server_socket, (char*)&game_state, sizeof(game_state), 0, (sockaddr*)&client.address, sizeof(client.address));
            
            
            num_client_action = 0;
            
            recv_timer = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
            
            while (num_client_action != CLIENT_QUANTITY)
            {
                FD_ZERO(&server_socket_read);
                FD_SET(server_socket, &server_socket_read);
                
                if (select(server_socket+1, &server_socket_read, NULL, NULL, &timeout) > 0)
                {
                    recvfrom(server_socket, &buffer, sizeof(char), 0, (sockaddr*)&client_address, &client_address_size);
                    
                    for (int j = 0; j < CLIENT_QUANTITY; ++j)
                    {
                        if (client_list[num_client_action].address.sin_addr.s_addr == client_address.sin_addr.s_addr)
                        {
                            action_list[num_client_action] = (PlayerAction)buffer;
                            ++num_client_action;
                            
                            recv_timer = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                            
                            break;
                        }
                        
                        if (chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() - recv_timer >= MAX_WAIT_SINGLE_CLIENT_MSEC)
                        {
                            ++num_client_action;
                            break;
                        }
                    }
                }
                else
                    break;
            }
            
            
            
            for (int i = 0; i < CLIENT_QUANTITY; ++i)
            {
                if   (client_list[i].is_alive) break; 
                if ((!client_list[i].is_alive) && (i == CLIENT_QUANTITY-1)) is_everyone_dead = true;
            }
       
       
       
            usleep(1000000/480); // Receives/sends approximately 480 packets per second
        }
    }
    
    
    
    WSACleanup();
    
    return 0;
}