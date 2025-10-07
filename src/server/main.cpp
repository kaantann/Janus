#include <thread>
#include <chrono>
#include <iostream>
#include <enet/enet.h>
#include "GameData.h"

int main()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return EXIT_FAILURE;
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 7777;

    ENetHost* server = enet_host_create(&address, 24, 1, 0, 0);

    if (server == NULL)
    {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Server started on port " << address.port << "\n";

    PlayerData player;
    player.position = {400, 300}; // Start player in the middle, since we use 800x600 fixed window for now.

    ENetEvent event;
    
    while (true)
    {
        while (enet_host_service(server, &event, 0) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                {
                    PlayerInput input = *(PlayerInput*)event.packet->data; //can be changed, does not have to be PlayerInput all the time.

                    float moveAmount = 5.0f;
                    if (input.keyW) player.position.y -= moveAmount;
                    if (input.keyS) player.position.y += moveAmount;
                    if (input.keyA) player.position.x -= moveAmount;
                    if (input.keyD) player.position.x += moveAmount;
                }
                
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client with " << event.peer->address.host << ":" << event.peer->address.port << " disconnected." << std::endl;
                break;
            }
        }

        // --- Broadcast Game State ---
        // Create a packet containing the player's current state
        ENetPacket* packet = enet_packet_create(&player, sizeof(PlayerData), ENET_PACKET_FLAG_RELIABLE);
        
        // Send the packet to all connected clients
        enet_host_broadcast(server, 0, packet);
        
        // We need to manually flush the host to send the packets out
        enet_host_flush(server);

        std::this_thread::sleep_for(std::chrono::milliseconds(32)); 
    }

    enet_host_destroy(server);
    enet_deinitialize();

    return 0;
}