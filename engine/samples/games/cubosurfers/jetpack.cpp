#include "jetpack.hpp"

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

static const Asset<Jetpack> JetpackAsset = AnyAsset("732498a6-7675-4efa-86ef-8f35d3bec485");
static const Asset<Player> PlayerAsset = AnyAsset("c7263b46-be18-47c2-b3ef-05592b2e9dec");
static const Asset<Player> PlayerNormalAsset = AnyAsset("57d1b886-8543-4b8b-8f78-d911e9c4f896");

CUBOS_REFLECT_IMPL(Jetpack)
{
    return cubos::core::ecs::TypeBuilder<Jetpack>("Jetpack")
        .withField("timer", &Jetpack::activeTime)
        .withField("velocity", &Jetpack::velocity)
        .withField("killZ", &Jetpack::killZ)
        .build();
}

void jetpackPlugin(cubos::engine::Cubos& cubos)
{
    cubos.depends(assetsPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(voxelsPlugin);
    cubos.depends(spawnerPlugin);
    cubos.depends(collisionsPlugin);
    // cubos.depends(obstaclePlugin);
    cubos.depends(playerPlugin);
    cubos.depends(timePlugin);

    cubos.component<Jetpack>();

    cubos.system("spawn Jetpack").call([](Commands cmds, const TotalTime& time, Query<Spawner&, Position&> spawn) {
        static float lastSpawn = 0.0F;
        float interval = 3.5F;
        for (auto [spawner, pos] : spawn)
        {
            if (time.shieldTime - lastSpawn > interval)
            {
                lastSpawn = time.shieldTime;

                Position spawnPosition = pos;
                int offset = (rand() % 3) - 1;
                spawnPosition.vec.x += static_cast<float>(offset) * spawner.laneWidth;
                auto ent = cmds.create()
                               .add<Jetpack>(Jetpack{3.0F})
                               .add<RenderVoxelGrid>(RenderVoxelGrid{JetpackAsset, glm::vec3{-3.0F, 0.0F, -1.0F}})
                               .add<Position>(Position{spawnPosition})
                               .add<Collider>(Collider{})
                               .add<BoxCollisionShape>(BoxCollisionShape{Box{glm::vec3{3.0F, 7.0F, 2.0F}}});
                CUBOS_INFO("Spawned Jetpack in lane {}", spawnPosition);
            }
        }
    });

    cubos.system("move Jetpack")
        .without<Player>()
        .call([](Commands cmds, const DeltaTime& dt, Query<Entity, Jetpack&, Position&> jetpack,
                 const TotalTime& totalTime) {
            for (auto [ent, jetpackComp, position] : jetpack)
            {
                jetpackComp.velocity.z -= (0.01F * totalTime.time);
                position.vec += jetpackComp.velocity * dt.value();
                position.vec.y = glm::abs(glm::sin(position.vec.z * 0.15F)) * 1.5F;
                if (position.vec.z < jetpackComp.killZ)
                {
                    cmds.destroy(ent);
                }
            }
        });

    cubos.system("pickup Jetpack")
        .call([](Commands cmds, Query<Entity, Player&, Position&, const CollidingWith&, Entity, Jetpack&> collisions) {
            for (auto [playerEnt, playerComp, playerPos, collidingWith, jetpackEnt, jetpackComp] : collisions)
            {
                CUBOS_ERROR("Player collided with Jetpack!");
                cmds.remove<RenderVoxelGrid>(playerEnt);
                cmds.add<Jetpack>(playerEnt, Jetpack{3.0F})
                    .add<RenderVoxelGrid>(playerEnt, RenderVoxelGrid{PlayerAsset, glm::vec3{-4.0F, 0.0F, -4.0F}});
                cmds.destroy(jetpackEnt);
                playerPos.vec.y = 1000.0F;
            }
        });

    cubos.system("update Jetpack")
        .with<Player>()
        .call([](Commands cmds, const DeltaTime& dt, Query<Entity, Player&, Jetpack&, Position&> jetpack) {
            for (auto [playerEnt, playerComp, jetpackComp, position] : jetpack)
            {
                jetpackComp.activeTime -= dt.value();
                if (jetpackComp.activeTime > 0.0F)
                {
                    position.vec.y = 1000.0F;
                }
                else
                {
                    cmds.remove<Jetpack>(playerEnt);
                    cmds.remove<RenderVoxelGrid>(playerEnt);
                    cmds.add<RenderVoxelGrid>(playerEnt,
                                              RenderVoxelGrid{PlayerNormalAsset, glm::vec3{-4.0F, 0.0F, -4.0F}});
                }
            }
        });
    return;
}
