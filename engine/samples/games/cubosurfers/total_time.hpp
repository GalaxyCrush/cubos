#pragma once

#include <glm/vec3.hpp>

#include <cubos/engine/prelude.hpp>

struct TotalTime
{
    CUBOS_REFLECT;
    float time{0.0F};
    float shieldTime{0.0F};
};

void timePlugin(cubos::engine::Cubos& cubos);