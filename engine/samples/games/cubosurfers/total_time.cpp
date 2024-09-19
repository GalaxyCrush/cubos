#include "total_time.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(TotalTime)
{
    return cubos::core::ecs::TypeBuilder<TotalTime>("TotalTime").withField("time", &TotalTime::time).build();
}

void timePlugin(cubos::engine::Cubos& cubos)
{

    cubos.resource<TotalTime>();
    cubos.system("Count the total time").call([](Commands cmds, const DeltaTime& dt, TotalTime& time) {
        time.time += dt.value();
        (void)cmds;
    });
}
