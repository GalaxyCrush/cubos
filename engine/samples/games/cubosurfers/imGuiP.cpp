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

#include "armor.hpp"
#include "jetpack.hpp"
#include "player.hpp"
#include "total_time.hpp"

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(UIGui)
{
    return cubos::core::ecs::TypeBuilder<UIGui>("UiGui").build();
}

void UIGuiPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(settingsPlugin);
    cubos.depends(windowPlugin);
    cubos.depends(renderTargetPlugin);
    cubos.depends(imguiPlugin);
    cubos.depends(timePlugin);
    cubos.depends(playerPlugin);
    cubos.depends(jetpackPlugin);
    cubos.depends(armorPlugin);

    cubos.resource<UIGui>();

    cubos.system("UI").tagged(imguiTag).call(
        [](Commands cmds, Query<Player&, Armor&> hasArmor, Query<Player&, Jetpack&> hasJetpack, TotalTime& time) {
            ImGui::Begin("UI");
            ImGui::SetWindowSize(ImVec2(200, 100));
            ImGui::Text("Score: %f", time.time);
            for (auto [player, armor] : hasArmor)
            {
                if (armor.active)
                {
                    ImGui::Text("Player has a Shield!!!");
                }
            }
            for (auto [player, jetpack] : hasJetpack)
            {
                ImGui::Text("Player has a Jetpack!!!");
            }
            ImGui::End();
            (void)cmds;
        });
    return;
}
