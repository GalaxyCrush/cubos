#include "plugin.hpp"
#include <random>

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/transform/plugin.hpp>

#include "../tile_map/plugin.hpp"


using namespace cubos::engine;

CUBOS_REFLECT_IMPL(demo::Walker)
{
    return cubos::core::ecs::TypeBuilder<Walker>("demo::Walker")
        .withField("position", &Walker::position)
        .withField("direction", &Walker::direction)
        .withField("facing", &Walker::facing)
        .withField("jumpHeight", &Walker::jumpHeight)
        .withField("moveSpeed", &Walker::moveSpeed)
        .withField("halfRotationTime", &Walker::halfRotationTime)
        .withField("minPosition", &Walker::minPosition)
        .withField("maxPosition", &Walker::maxPosition)
        .withField("progress", &Walker::progress)
        .build();
}

void demo::walkerPlugin(Cubos& cubos)
{
    cubos.depends(tileMapPlugin);
    cubos.depends(transformPlugin);

    cubos.component<Walker>();

    cubos.system("do Walker movement")
        .before(transformUpdateTag)
        .call([](const DeltaTime& dt, Query<Entity, Position&, Rotation&, Walker&, const ChildOf&, TileMap&> query) {
            for (auto [ent, position, rotation, walker, childOf, map] : query)
            {
                auto tileSide = static_cast<float>(map.tileSide);

                if (!walker.initialized)
                {
                    // If the initial position is already occupied, try to find an empty one nearby.
                    walker.position.x = glm::clamp(walker.position.x, 0, static_cast<int>(map.floorTiles.size() - 1));
                    walker.position.y = glm::clamp(walker.position.y, 0, static_cast<int>(map.floorTiles.size() - 1));
                    while (!map.entities[walker.position.y][walker.position.x].isNull())
                    {
                        walker.position += glm::ivec2{1, 0};
                        int r = rand() % 2;
                        walker.position += (rand() % 2 == 0 ? -1 : 1) * glm::ivec2{r, 1 - r};
                        if (walker.position.x >= static_cast<int>(map.floorTiles.size()))
                        {
                            walker.position.x -= static_cast<int>(map.floorTiles.size());
                        }
                        if (walker.position.y >= static_cast<int>(map.floorTiles.size()))
                        {
                            walker.position.y -= static_cast<int>(map.floorTiles.size());
                        }
                    }
                    map.entities[walker.position.y][walker.position.x] = ent;

                    // Set the actual position of the entity.
                    position.vec.x = tileSide / 2.0F + tileSide * static_cast<float>(walker.position.x);
                    position.vec.y = 0.0F;
                    position.vec.z = tileSide / 2.0F + tileSide * static_cast<float>(walker.position.y);
                    walker.initialized = true;
                }

                auto targetTile = walker.position + walker.direction;
                if (walker.direction != glm::ivec2{0, 0})
                {
                    walker.facing = walker.direction;

                    // Check if movement is valid.
                    if (targetTile.x < 0 || targetTile.y < 0 || targetTile.x >= map.floorTiles.size() ||
                        targetTile.y >= map.floorTiles.size() || targetTile.x < walker.minPosition.x ||
                        targetTile.y < walker.minPosition.y || targetTile.x > walker.maxPosition.x ||
                        targetTile.y > walker.maxPosition.y ||
                        (!map.entities[targetTile.y][targetTile.x].isNull() &&
                         map.entities[targetTile.y][targetTile.x] != ent))
                    {
                        // There's already an entity in the target tile, or its out of bounsd, stop the movement.
                        walker.direction = {0, 0};
                    }
                }

                if (walker.direction != glm::ivec2{0, 0})
                {
                    // Occupy the target tile.
                    map.entities[targetTile.y][targetTile.x] = ent;

                    // Get the source and target positions of the entity's movement.
                    glm::vec2 source = static_cast<glm::vec2>(walker.position);
                    glm::vec2 target = static_cast<glm::vec2>(walker.position + walker.direction);

                    // Increase the progress value and calculate the new position of the entity.
                    walker.progress = glm::clamp(walker.progress + dt.value() * walker.moveSpeed, 0.0F, 1.0F);
                    position.vec.x = tileSide / 2.0F + tileSide * glm::mix(source.x, target.x, walker.progress);
                    position.vec.y = glm::mix(0.0F, walker.jumpHeight, glm::sin(walker.progress * glm::pi<float>()));
                    position.vec.z = tileSide / 2.0F + tileSide * glm::mix(source.y, target.y, walker.progress);

                    // If the entity has reached the target position, reset the direction.
                    if (walker.progress == 1.0F)
                    {
                        // Clear the source tile.
                        map.entities[walker.position.y][walker.position.x] = Entity{};

                        walker.position += walker.direction;
                        walker.direction = {0, 0};
                        walker.progress = 0.0F;
                    }
                }

                // Set the entity's rotation as appropriate.
                auto targetRotation =
                    glm::quatLookAt(-glm::normalize(glm::vec3(static_cast<float>(walker.facing.x), 0.0F,
                                                              static_cast<float>(walker.facing.y))),
                                    glm::vec3{0.0F, 1.0F, 0.0F});
                float rotationAlpha = 1.0F - glm::pow(0.5F, dt.value() / walker.halfRotationTime);
                rotation.quat = glm::slerp(rotation.quat, targetRotation, rotationAlpha);
            }
        });

    cubos.observer("clear up Walker positions")
        .onRemove<Walker>()
        .call([](Query<Entity, Walker&, const ChildOf&, TileMap&> query) {
            for (auto [ent, walker, childOf, map] : query)
            {
                // Mark the tile as free.
                CUBOS_ASSERT(map.entities[walker.position.y][walker.position.x] == ent,
                             "Tile is not occupied by the entity");
                map.entities[walker.position.y][walker.position.x] = {};

                auto target = walker.position + walker.direction;
                if (target.x >= 0 && target.y >= 0 && target.x < map.floorTiles.size() &&
                    target.y < map.floorTiles.size() && map.entities[target.y][target.x] == ent)
                {
                    map.entities[target.y][target.x] = {};
                }
            }
        });
}
