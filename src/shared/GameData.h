#pragma once

#include "glm/glm.hpp"

struct PlayerData
{
    glm::vec2 position;
};

struct PlayerInput
{
    bool keyW = false;
    bool keyA = false;
    bool keyS = false;
    bool keyD = false;
};