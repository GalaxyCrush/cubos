#include "obstacle.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include "total_time.hpp"
using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Obstacle)
{
    return cubos::core::ecs::TypeBuilder<Obstacle>("Obstacle")
        .withField("velocity", &Obstacle::velocity)
        .withField("killZ", &Obstacle::killZ)
        .build();
}

void obstaclePlugin(Cubos& cubos)
{
    cubos.depends(assetsPlugin);
    cubos.depends(transformPlugin);
    cubos.depends(timePlugin);

    cubos.component<Obstacle>();

    cubos.system("move obstacles")
        .call([](Commands cmds, const DeltaTime& dt, Query<Entity, Obstacle&, Position&> obstacles,
                 TotalTime& totalTime) {
            for (auto [ent, obstacle, position] : obstacles)
            {
                obstacle.velocity.z -= (0.01F * totalTime.time);
                position.vec += obstacle.velocity * dt.value();
                position.vec.y = glm::abs(glm::sin(position.vec.z * 0.15F)) * 1.5F;
                // CUBOS_INFO("Obstacle velocity: ({}, {}, {}); Velocity: {}", obstacle.velocity.x, obstacle.velocity.y,
                //            obstacle.velocity.z, totalTime.time);
                if (position.vec.z < obstacle.killZ)
                {
                    cmds.destroy(ent);
                }
            }
        });
}
