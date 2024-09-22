#include "imGuiP.hpp"
#include <iostream>
#include <map>
#include <vector>

#include <imgui.h>

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/geom/box.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/map.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>
#include <cubos/core/reflection/external/vector.hpp>
#include <cubos/core/reflection/reflect.hpp>
#include <cubos/core/reflection/traits/constructible.hpp>
#include <cubos/core/reflection/traits/fields.hpp>
#include <cubos/core/reflection/traits/nullable.hpp>

#include <cubos/engine/assets/assets.hpp>
#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/collider.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/engine/collisions/shapes/box.hpp>
#include <cubos/engine/imgui/data_inspector.hpp>
#include <cubos/engine/imgui/plugin.hpp>
#include <cubos/engine/render/target/plugin.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>
#include <cubos/engine/window/plugin.hpp>
using namespace cubos::engine;

CUBOS_REFLECT_IMPL(UIGui)
{
    return cubos::core::ecs::TypeBuilder<UIGui>("UiGui").withField("info", &UIGui::info).build();
}

void UIGuiPlugin(cubos::engine::Cubos& cubos)
{

    cubos.resource<UIGui>();

    cubos.system("UI").call([](Commands cmds, UIGui& ui) {
        ImGui::Begin("UI");
        ImGui::Text("Info: %s", ui.info.c_str());
        ImGui::End();
        (void)cmds;
    });
    return;
}
