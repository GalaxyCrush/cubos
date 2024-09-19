#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/defaults/plugin.hpp>
#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/render/lights/environment.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/scene/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/tools/plugin.hpp>
#include <cubos/engine/utils/free_camera/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

#include "obstacle.hpp"
#include "player.hpp"
#include "spawner.hpp"
#include "total_time.hpp"

using namespace cubos::engine;

static const Asset<Scene> SceneAsset = AnyAsset("ee5bb451-05b7-430f-a641-a746f7009eef");
static const Asset<VoxelPalette> PaletteAsset = AnyAsset("101da567-3d23-46ae-a391-c10ec00e8718");
static const Asset<InputBindings> InputBindingsAsset = AnyAsset("b20900a4-20ee-4caa-8830-14585050bead");
int main()
{
    Cubos cubos{argc, argv};

    cubos.plugin(timePlugin);
    cubos.plugin(defaultsPlugin);
    cubos.plugin(freeCameraPlugin);
    cubos.plugin(toolsPlugin);
    cubos.plugin(spawnerPlugin);
    cubos.plugin(obstaclePlugin);
    cubos.plugin(playerPlugin);

    cubos.startupSystem("configure settings").before(settingsTag).call([](Settings& settings) {
        settings.setString("assets.app.osPath", APP_ASSETS_PATH);
        settings.setString("assets.builtin.osPath", BUILTIN_ASSETS_PATH);
    });

    cubos.startupSystem("set the palette, environment, input bindings and spawn the scene")
        .tagged(assetsTag)
        .call([](Commands commands, const Assets& assets, RenderPalette& palette, Input& input,
                 RenderEnvironment& environment) {
            palette.asset = PaletteAsset;
            environment.ambient = {0.1F, 0.1F, 0.1F};
            environment.skyGradient[0] = {0.2F, 0.4F, 0.8F};
            environment.skyGradient[1] = {0.6F, 0.6F, 0.8F};
            input.bind(*assets.read(InputBindingsAsset));
            commands.spawn(assets.read(SceneAsset)->blueprint);
        });

    cubos.system("restart the game on input")
        .call([](Commands cmds, const Assets& assets, const Input& input, Query<Entity> all, TotalTime& time) {
            if (input.justPressed("restart"))
            {
                for (auto [ent] : all)
                {
                    cmds.destroy(ent);
                }
                time.time = 0.0F;
                cmds.spawn(assets.read(SceneAsset)->blueprint);
            }
        });

    cubos.system("detect player vs obstacle collisions")
        .call([](Commands cmds, const Assets& assets,
                 Query<const Player&, const CollidingWith&, const Obstacle&> collisions, Query<Entity> all,
                 TotalTime& time) {
            for (auto [player, collidingWith, obstacle] : collisions)
            {
                for (auto [ent] : all)
                {
                    cmds.destroy(ent);
                }
                CUBOS_INFO("Player collided with an obstacle!");
                (void)player; // here to shut up 'unused variable warning', you can remove it
                time.time = 0.0F;
                cmds.spawn(assets.read(SceneAsset)->blueprint);
            }
        });
    cubos.run();
}
