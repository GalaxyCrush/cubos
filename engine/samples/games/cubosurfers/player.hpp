#pragma once

#include <cubos/engine/prelude.hpp>

#include "armor.hpp"

struct Player
{
    CUBOS_REFLECT;

    float speed{1.0F};     // Speed of the player
    float laneWidth{1.0F}; // Width of the lane
    int lane{0};           // Current lane
    int targetLane{0};     // Target lane
    float isUp{0.0F};      // Is the player in the sky
};

void playerPlugin(cubos::engine::Cubos& cubos);
