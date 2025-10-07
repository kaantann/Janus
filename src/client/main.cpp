// It is FORBIDDEN from including enet.h.
#include "raylib.h"
#include "NetworkManager.h" 
#include "GameData.h"       
#include <iostream>

int main(void)
{
    int const screenWidth = 800;
    int const screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Client");
    SetTargetFPS(60);

    if (!Network_Initialize())
    {
        std::cerr << "Failed to initialize network." << std::endl;
        CloseWindow();
        return -1;
    }

    PlayerData player;
    player.position = {-100, -100}; // Player starts off-screen
    glm::vec2 playerSize = {50.0f, 50.0f};

    bool hasReceivedFirstState = false;

    while (!WindowShouldClose())
    {
        while (Network_ReceiveState(player))
        {
            hasReceivedFirstState = true;
        }

        PlayerInput input = {0};
        input.keyW = IsKeyDown(KEY_W);
        input.keyS = IsKeyDown(KEY_S);
        input.keyA = IsKeyDown(KEY_A);
        input.keyD = IsKeyDown(KEY_D);

        Network_SendInput(input);

        BeginDrawing();
        ClearBackground(BLACK);

        // Only draw the player if we have received at least one update from the server
        if (hasReceivedFirstState)
        {
            DrawRectangleV({player.position.x, player.position.y}, {playerSize.x, playerSize.y}, WHITE);
        }

        DrawText("Are you smooth?", 10, 10, 20, LIGHTGRAY);
        EndDrawing();
    }

    Network_Shutdown();
    CloseWindow();

    return 0;
}