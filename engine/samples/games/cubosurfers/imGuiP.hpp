#pragma once

#include <string>

#include <glm/vec3.hpp>

#include <cubos/engine/prelude.hpp>

struct UIGui
{
    CUBOS_REFLECT;
    std::string info;
};

void UIGuiPlugin(cubos::engine::Cubos& cubos);