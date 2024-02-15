#include <winsock2.h>

#include <string.h>

#include <unistd.h>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>  

#include "multiplayer_datatypes.cpp"


void StartMultiplayerGame()
{   
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    
    
    SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    
    
    // Disabling connection reset error: https://stackoverflow.com/questions/34242622/windows-udp-sockets-recvfrom-fails-with-error-10054
    bool error_enabled = false;
    DWORD output = 0;
    WSAIoctl(client_socket, _WSAIOW(IOC_VENDOR, 12) /*SIO_UDP_CONNRESET*/, &error_enabled, sizeof(error_enabled), NULL, 0, &output, NULL, NULL);
    
    
    char original_ip[31] = {'\0'};
    char ip[31] = {'\0'};
    
    ifstream ip_file;
    ip_file.open("./server_settings.txt");
    
    if ((ip_file.is_open()) && !(ip_file.peek() == ifstream::traits_type::eof()))
    {
        ip_file >> ip;
        ip_file.close();
    }
    else 
        strcpy(ip, "127.0.0.1");
    
    strcpy(original_ip, ip);
    
    Texture2D egg = LoadTexture("./resources/sprites/ui/egg.png");
    
    sockaddr_in server_address;
    
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(43424);
    server_address.sin_addr.s_addr = inet_addr(ip);
    
    int server_address_size = sizeof(server_address);
    
    
    
    Texture2D background = LoadTexture("./resources/sprites/scenes/background.png");
    Font font = LoadFont("./resources/fonts/queroquero.ttf");
    
    PlayerAction action = NONE;
    
    
    int frame = 0;
    
    
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = MAX_WAIT_USEC;
    
    fd_set client_socket_read;
    
    char char_pressed;
    int length;
    
    InitialPacket info;
    
    time_t start_time;
	int    self_index = -1;
    
    while ((!WindowShouldClose()) && (self_index == -1))
    {
        BeginDrawing();
        
            DrawTexture(background, 0, 0, WHITE);
            
            DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0,0,0,128});
            
            DrawTextEx(font, ip, {96, 635}, 32, 0, WHITE);
            DrawTextEx(font, "PRESS ENTER TO JOIN", {96, 659}, 64, 0, WHITE);
            
        EndDrawing();
        
        char_pressed = GetKeyPressed();
        if (((IsKeyPressed(KEY_PERIOD)) || ((char_pressed >= 48) && (char_pressed <= 57))) && (strlen(ip) != 30))
        {
            length       = strlen(ip);
            ip[length]   = char_pressed;
            ip[length+1] = '\0';
        }
        else if ((IsKeyPressed(KEY_BACKSPACE)) && (strlen(ip) > 0))
            ip[strlen(ip)-1] = '\0';
        

        
        if (IsKeyPressed(KEY_ENTER))
        {
            if (strcmp(original_ip, ip) != 0)
            {  
                ofstream ip_file;
                ip_file.open("./server_settings.txt", ios::out | ios::trunc);
                
                ip_file << ip;
                
                ip_file.close();
            }
            
            start_time = time(NULL);
            frame      = 0;
            
            while (time(NULL) - start_time < 8)
            {
                ++frame;
                
                BeginDrawing();
                
                    DrawTexture(background, 0, 0, WHITE);
            
                    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0,0,0,128});
            
                    DrawTextEx(font, ip, {96, 635}, 32, 0, WHITE);
                    DrawTextEx(font, "TRYING TO CONNECT...", {96, 659}, 64, 0, WHITE);
                        
                    /*    
                    for (int i = 0; i < 5; ++i)
                        if ((int)(frame/10)%5 >= (i+1))
                           DrawTexture(egg, 50 + i * 75, 50, WHITE);
                    */
                            
                EndDrawing();
                
                
                sendto(client_socket, (char*)&action, sizeof(PlayerAction), 0, (sockaddr*)&server_address, sizeof(server_address));
                
                
                FD_ZERO(&client_socket_read);
                FD_SET(client_socket, &client_socket_read);
                
                if (select(client_socket+1, &client_socket_read, NULL, NULL, &timeout) > 0)
                {
                    recvfrom(client_socket, (char*)&info, sizeof(InitialPacket), 0, (sockaddr*)&server_address, &server_address_size);
                    self_index = info.client_index;
                    
                    break;
                }
                
                if (time(NULL) - start_time >= 8)
                {
                    while (time(NULL) - start_time <= 11)
                    {
                       BeginDrawing();
                    
                           DrawTexture(background, 0, 0, WHITE);
            
                           DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, {0,0,0,128});
            
                           DrawTextEx(font, ip, {96, 635}, 32, 0, WHITE);
                           DrawTextEx(font, "CONNECTION FAILED", {96, 659}, 64, 0, RED);
                                
                        EndDrawing(); 
                    }
                }
            }
        }
    }
    
    
    
    Texture2D bird_texture = LoadTexture("./resources/sprites/characters/queroquero.png"); 
    
	Player player_list[info.client_quantity];
	for (int i = 0; i < info.client_quantity; ++i)
    {
        player_list[i].sprite          = bird_texture;
        player_list[i].size_multiplier = 1.0;
    }
    
    
    Texture2D obs_texture = LoadTexture("./resources/sprites/obstacles/pipe.png");
    
    Obstacle obs_list[OBSTACLE_QUANTITY];
    for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
        obs_list[i].sprite = obs_texture;

    
    
    Packet game_state;
    
    while (!WindowShouldClose())
    {
        ++frame;
        
        
        FD_ZERO(&client_socket_read);
        FD_SET(client_socket, &client_socket_read);
                
        if (select(client_socket+1, &client_socket_read, NULL, NULL, &timeout) > 0)
            recvfrom(client_socket, (char*)&game_state, sizeof(Packet) + (sizeof(PlayerInfo) * MAX_CLIENTS) + (sizeof(ObstacleInfo) * OBSTACLE_QUANTITY), 0, (sockaddr*)&server_address, &server_address_size);
        
        
		
		for (int i = 0; i < info.client_quantity; ++i)
		{
			player_list[i].position_x = game_state.player_list[i].position_x;
			player_list[i].position_y = game_state.player_list[i].position_y;
			player_list[i].speed      = game_state.player_list[i].speed;
			player_list[i].color      = game_state.player_list[i].color;
		}
        
        for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
		{
			obs_list[i].position_x     = game_state.obs_list[i].position_x;
			obs_list[i].gap_height     = game_state.obs_list[i].gap_height;
			obs_list[i].gap_position_y = game_state.obs_list[i].gap_position_y;
		}
		
        
        
        BeginDrawing();
        
            DrawTexture(background, 0, 0, WHITE);
            
            // Draws the other clients behind the local one and slightly transparent
            for (int i = 0; i < info.client_quantity; ++i)
            {
                if (i != self_index)
                {
                    player_list[i].color.a = 128;
                    DrawPlayer(player_list[i], frame);
                }
            }
            
            DrawPlayer(player_list[self_index], frame);
			
			for (int i = 0; i < OBSTACLE_QUANTITY; ++i)
                DrawObstacle(obs_list[i]);
            
        EndDrawing();   
        
        
        
        action = IsKeyPressed(KEY_SPACE) ? JUMP : NONE;    
        sendto(client_socket, (char*)&action, sizeof(PlayerAction), 0, (sockaddr*)&server_address, sizeof(server_address));
    }
    
    WSACleanup();
    
    
    UnloadTexture(background);
    UnloadTexture(bird_texture);
    UnloadTexture(obs_texture);
}

