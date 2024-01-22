
#include <winsock2.h>

#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;



typedef struct Client {
    sockaddr_in address;
    Player player;
} Client;



int StartMultiplayerGame()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    
    SOCKET client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in server_address;
    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port        = htons(43424);
    int server_address_size = sizeof(server_address);

    Client client_list[2];
    
    
    char jump_event = 0;
    Texture2D character = LoadTexture("./resources/sprites/characters/queroquero.png"); //MUDARTEMPORARIOOOOOOOOOOOOOOOOOOOOOOO
    while (true)
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
    
    recvfrom(client_socket, (char*)client_list, sizeof(Client)*2, 0, (sockaddr*)&server_address, &server_address_size);
    for (Client client : client_list)
        client.player.sprite = character;

    
    while (!WindowShouldClose())
    {
        ++frame;
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("antonio teixeira", 400, 400, 30, RED);
            if (IsKeyPressed(KEY_SPACE)) jump_event = 1;
            sendto(client_socket, &jump_event, sizeof(jump_event), 0, (sockaddr*)&server_address, sizeof(server_address));
            jump_event = 0;
            for (Client client : client_list)
                DrawPlayer(&client.player, frame);

            
        EndDrawing();
        recvfrom(client_socket, (char*)client_list, sizeof(Client)*2, 0, (sockaddr*)&server_address, &server_address_size);
    }
    
    
    
    WSACleanup();
    
    return 0;
}