# Flappy Quero Quero

![queroquero_single](https://github.com/cauardsilva/flappy_inf/assets/32238723/0beea631-44f9-470b-9cd3-78c362310efc)

This is essentially a Windows-only game (it may work in other platforms with some light adjustments, except the multiplayer functionalities because of Winsock2 --> comment the StartMultiplayerGame() inside main.cpp and remove the #include "./scripts/multiplayer_game.cpp").


If you want to modify/compile the code, you will need Raylib (the graphics library used) installed in your computer (and you may need to modify some of names inside Raylib or Windows native libraries because of some conflicts).
The ones I remember from the top of my head are Rectangle, ShowCursor, CloseWindow and byte.

https://www.raylib.com/


