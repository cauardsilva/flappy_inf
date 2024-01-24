
#include <winsock2.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>  

using namespace std;



typedef struct Client {
    sockaddr_in address;
    Player player;
    Color color;
} Client;






int StartMultiplayerGame()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    
    srand(time(NULL));
    
    SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    
    
    
    char ip[32] = {'\0'};
    
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(43424);
    
    ifstream ip_file;
    ip_file.open("./server_settings.txt");
    
    if ((ip_file.is_open()) && !(ip_file.peek() == ifstream::traits_type::eof()))
    {
        ip_file >> ip;
        ip_file.close();
        
        server_address.sin_addr.s_addr = inet_addr(ip);
    }
    else 
        server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    
    
    


    
    
    int server_address_size = sizeof(server_address);



    
    
    
    char jump_event = 0;
    Texture2D character = LoadTexture("./resources/sprites/characters/queroquero.png"); //MUDARTEMPORARIOOOOOOOOOOOOOOOOOOOOOOO
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("marcos dutra", 400, 400, 30, RED);
            if (IsKeyPressed(KEY_ENTER))
            {    
                sendto(client_socket, &jump_event, sizeof(jump_event), 0, (sockaddr*)&server_address, sizeof(server_address));
                break;
            }
        EndDrawing();
    }
    int frame = 0;
    
    int client_quantity = 0;
    recvfrom(client_socket, (char*)&client_quantity, sizeof(int), 0, (sockaddr*)&server_address, &server_address_size);

    
    Client client_list[client_quantity];
    
    // eh resetado todo frame pq override clientlistpelorecebido
    //
    //for (int i = 0; i < client_quantity; ++i)
    //    client_list[i].player.sprite = character;
    //
    
    
    while (!WindowShouldClose())
    {
        ++frame;


        recvfrom(client_socket, (char*)client_list, sizeof(Client) * client_quantity, 0, (sockaddr*)&server_address, &server_address_size);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("antonio teixeira", 400, 400, 30, RED);
            
    
            jump_event = IsKeyPressed(KEY_SPACE) ? 1 : 0;
            sendto(client_socket, &jump_event, sizeof(jump_event), 0, (sockaddr*)&server_address, sizeof(server_address));

            for (int i = 0; i < client_quantity; ++i){
                client_list[i].player.sprite = character;
                DrawPlayer(&(client_list[i].player), frame, 12, 1.0, client_list[i].color);
            }
            
        EndDrawing();   
    }
    
    
    
    WSACleanup();
    
    return 0;
}

