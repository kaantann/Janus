#pragma once
#include "GameData.h" // Include our shared data structures

// Initialize the network connection to the server.
bool Network_Initialize();

// Shut down the network connection.
void Network_Shutdown();

// Send the player's current input to the server.
void Network_SendInput(const PlayerInput& input);

// Check for new messages from the server.
// If a game state packet is received, it updates out_playerData and returns true.
bool Network_ReceiveState(PlayerData& out_playerData);