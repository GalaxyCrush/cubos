
#include "armor.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/geom/box.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/assets.hpp>
#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/collider.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/collisions/plugin.hpp>
#include <cubos/engine/collisions/shapes/box.hpp>
#include <cubos/engine/render/voxels/grid.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

#include "obstacle.hpp"
#include "player.hpp"
#include "spawner.hpp"
#include "total_time.hpp"

using namespace cubos::engine;
using namespace cubos::core::geom;

static const Asset<Armor> ArmorAsset = AnyAsset("fd89da0f-72d2-4ef9-9297-f73ff2ca5c63");
static const Asset<Player> PlayerAsset = AnyAsset("4892c2f3-10b3-4ca7-9de3-822b77a0ba7e");

CUBOS_REFLECT_IMPL(Armor)
{
    return cubos::core::ecs::TypeBuilder<Armor>("Armor")
        .withField("active", &Armor::active)
        .withField("velocity", &Armor::velocity)
        .withField("killZ", &Armor::killZ)
        .build();
}

void armorPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(assetsPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(voxelsPlugin);
    cubos.depends(spawnerPlugin);
    cubos.depends(collisionsPlugin);
    // cubos.depends(obstaclePlugin);
    cubos.depends(playerPlugin);
    cubos.depends(timePlugin);

    cubos.component<Armor>();

    cubos.system("spawn armor").call([](Commands cmds, const TotalTime& time, Query<Spawner&, Position&> spawn) {
        static float lastSpawn = 0.0F;
        float interval = 2.0F;
        for (auto [spawner, pos] : spawn)
        {
            if (time.shieldTime - lastSpawn > interval)
            {
                lastSpawn = time.shieldTime;

                Position spawnPosition = pos;
                int offset = (rand() % 3) - 1;
                spawnPosition.vec.x += static_cast<float>(offset) * spawner.laneWidth;
                auto ent = cmds.create()
                               .add<Armor>(Armor{true})
                               .add<RenderVoxelGrid>(RenderVoxelGrid{ArmorAsset, glm::vec3{-3.0F, 0.0F, -1.0F}})
                               .add<Position>(Position{spawnPosition})
                               .add<Collider>(Collider{})
                               .add<BoxCollisionShape>(BoxCollisionShape{Box{glm::vec3{3.0F, 7.0F, 2.0F}}});
                CUBOS_INFO("Spawned armor in lane {}", spawnPosition);
            }
        }
    });

    cubos.system("move armor")
        .without<Player>()
        .call(
            [](Commands cmds, const DeltaTime& dt, Query<Entity, Armor&, Position&> armor, const TotalTime& totalTime) {
                for (auto [ent, armorComp, position] : armor)
                {
                    armorComp.velocity.z -= (0.01F * totalTime.time);
                    position.vec += armorComp.velocity * dt.value();
                    position.vec.y = glm::abs(glm::sin(position.vec.z * 0.15F)) * 1.5F;
                    if (position.vec.z < armorComp.killZ)
                    {
                        cmds.destroy(ent);
                    }
                }
            });

    cubos.system("pickup armor")
        .call([](Commands cmds, Query<Entity, Player&, const CollidingWith&, Entity, Armor&> collisions) {
            for (auto [playerEnt, playerComp, collidingWith, armorEnt, armorComp] : collisions)
            {
                CUBOS_ERROR("Player collided with armor!");
                cmds.remove<RenderVoxelGrid>(playerEnt);
                cmds.add<Armor>(playerEnt, Armor{true})
                    .add<RenderVoxelGrid>(playerEnt, RenderVoxelGrid{PlayerAsset, glm::vec3{-4.0F, 0.0F, -4.0F}});
                cmds.destroy(armorEnt);
            }
        });
}
