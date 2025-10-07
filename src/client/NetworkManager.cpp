#include "NetworkManager.h"
#include <enet/enet.h>
#include <iostream>

static ENetHost* client = nullptr;
static ENetPeer* peer = nullptr;

bool Network_Initialize()
{
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        return false;
    }

    client = enet_host_create(NULL, 1, 1, 0, 0);
    if (client == NULL)
    {
        std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 7777;

    peer = enet_host_connect(client, &address, 1, 0);
    if (peer == NULL)
    {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        return false;
    }

    std::cout << "Connecting to server..." << std::endl;
    return true;
}

void Network_Shutdown()
{
    if (peer)
    {
        enet_peer_disconnect(peer, 0);
    }
    if (client)
    {
        enet_host_destroy(client);
    }
    enet_deinitialize();
}

void Network_SendInput(const PlayerInput& input)
{
    if (!peer) 
        return;

    ENetPacket* packet = enet_packet_create(&input, sizeof(PlayerInput), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

bool Network_ReceiveState(PlayerData& out_playerData)
{
    if (!client) 
        return false;

    bool isNewStateReceived{false};
    ENetEvent event;
    while (enet_host_service(client, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                // When we get a packet, we assume it's the server's authoritative PlayerData
                out_playerData = *(PlayerData*)event.packet->data;
                enet_packet_destroy(event.packet);
                isNewStateReceived = true;
                break;
        }
    }
    return isNewStateReceived;
}