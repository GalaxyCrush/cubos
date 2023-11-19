#include <cubos/core/ecs/component/registry.hpp>
#include <cubos/core/ecs/world.hpp>
#include <cubos/core/log.hpp>
#include <cubos/core/reflection/external/string.hpp>
#include <cubos/core/reflection/type.hpp>

using namespace cubos::core;
using namespace cubos::core::ecs;

World::World(std::size_t initialCapacity)
    : mEntityManager(initialCapacity)
{
    // Do nothing.
    // Edu: BAH!
}

Entity World::create()
{
    Entity::Mask mask{};
    mask.set(0);
    auto entity = mEntityManager.create(mask);
    CUBOS_DEBUG("Created entity {}", entity);
    return entity;
}

void World::destroy(Entity entity)
{
    mEntityManager.destroy(entity);
    mComponentManager.removeAll(entity.index);
    CUBOS_DEBUG("Destroyed entity {}", entity);
}

bool World::isAlive(Entity entity) const
{
    return mEntityManager.isAlive(entity);
}

World::Components World::components(Entity entity)
{
    return Components{*this, entity};
}

World::ConstComponents World::components(Entity entity) const
{
    return ConstComponents{*this, entity};
}

World::Iterator World::begin() const
{
    return mEntityManager.begin();
}

World::Iterator World::end() const
{
    return mEntityManager.end();
}

World::Components::Components(World& world, Entity entity)
    : mWorld{world}
    , mEntity{entity}
{
    CUBOS_ASSERT(world.isAlive(entity));
}

bool World::Components::has(const reflection::Type& type) const
{
    std::size_t componentId = mWorld.mComponentManager.getID(type);
    return mWorld.mEntityManager.getMask(mEntity).test(componentId);
}

void* World::Components::get(const reflection::Type& type)
{
    CUBOS_ASSERT(this->has(type));
    std::size_t componentId = mWorld.mComponentManager.getID(type);
    return mWorld.mComponentManager.storage(componentId)->get(mEntity.index);
}

auto World::Components::begin() -> Iterator
{
    return Iterator{*this, false};
}

auto World::Components::end() -> Iterator
{
    return Iterator{*this, true};
}

auto World::Components::add(const reflection::Type& type, void* value) -> Components&
{
    std::size_t componentId = mWorld.mComponentManager.getID(type);
    auto mask = mWorld.mEntityManager.getMask(mEntity);
    mask.set(componentId);
    mWorld.mEntityManager.setMask(mEntity, mask);
    mWorld.mComponentManager.add(mEntity.index, type, value);

    CUBOS_DEBUG("Added component {} to entity {}", type.name(), mEntity, mEntity);
    return *this;
}

auto World::Components::remove(const reflection::Type& type) -> Components&
{
    std::size_t componentId = mWorld.mComponentManager.getID(type);
    auto mask = mWorld.mEntityManager.getMask(mEntity);
    mask.set(componentId, false);
    mWorld.mEntityManager.setMask(mEntity, mask);
    mWorld.mComponentManager.remove(mEntity.index, componentId);

    CUBOS_DEBUG("Removed component {} from entity {}", type.name(), mEntity);
    return *this;
}

World::ConstComponents::ConstComponents(const World& world, Entity entity)
    : mWorld{world}
    , mEntity{entity}
{
    CUBOS_ASSERT(world.isAlive(entity));
}

bool World::ConstComponents::has(const reflection::Type& type) const
{
    std::size_t componentId = mWorld.mComponentManager.getID(type);
    return mWorld.mEntityManager.getMask(mEntity).test(componentId);
}

const void* World::ConstComponents::get(const reflection::Type& type) const
{
    CUBOS_ASSERT(this->has(type));
    std::size_t componentId = mWorld.mComponentManager.getID(type);
    return mWorld.mComponentManager.storage(componentId)->get(mEntity.index);
}

auto World::ConstComponents::begin() const -> Iterator
{
    return Iterator{*this, false};
}

auto World::ConstComponents::end() const -> Iterator
{
    return Iterator{*this, true};
}

World::Components::Iterator::Iterator(Components& components, bool end)
    : mComponents{components}
{
    const auto& mask = mComponents.mWorld.mEntityManager.getMask(mComponents.mEntity);

    if (end)
    {
        mId = mask.size();
    }

    while (mId < mask.size() && !mask.test(mId))
    {
        mId += 1;
    }
}

bool World::Components::Iterator::operator==(const Iterator& other) const
{
    return mId == other.mId && mComponents.mEntity == other.mComponents.mEntity;
}

auto World::Components::Iterator::operator*() const -> const Component&
{
    const auto& mask = mComponents.mWorld.mEntityManager.getMask(mComponents.mEntity);
    CUBOS_ASSERT(mId < mask.size(), "Iterator is out of bounds");
    mComponent.type = &mComponents.mWorld.mComponentManager.getType(mId);
    mComponent.value = mComponents.get(*mComponent.type);
    return mComponent;
}

auto World::Components::Iterator::operator->() const -> const Component*
{
    return &this->operator*();
}

auto World::Components::Iterator::operator++() -> Iterator&
{
    const auto& mask = mComponents.mWorld.mEntityManager.getMask(mComponents.mEntity);
    CUBOS_ASSERT(mId < mask.size(), "Iterator is out of bounds");
    do
    {
        mId += 1;
    } while (mId < mask.size() && !mask.test(mId));

    return *this;
}

World::ConstComponents::Iterator::Iterator(const ConstComponents& components, bool end)
    : mComponents{components}
{
    const auto& mask = mComponents.mWorld.mEntityManager.getMask(mComponents.mEntity);

    if (end)
    {
        mId = mask.size();
    }

    while (mId < mask.size() && !mask.test(mId))
    {
        mId += 1;
    }
}

bool World::ConstComponents::Iterator::operator==(const Iterator& other) const
{
    return mId == other.mId && mComponents.mEntity == other.mComponents.mEntity;
}

auto World::ConstComponents::Iterator::operator*() const -> const Component&
{
    const auto& mask = mComponents.mWorld.mEntityManager.getMask(mComponents.mEntity);
    CUBOS_ASSERT(mId < mask.size(), "Iterator is out of bounds");
    mComponent.type = &mComponents.mWorld.mComponentManager.getType(mId);
    mComponent.value = mComponents.get(*mComponent.type);
    return mComponent;
}

auto World::ConstComponents::Iterator::operator->() const -> const Component*
{
    return &this->operator*();
}

auto World::ConstComponents::Iterator::operator++() -> Iterator&
{
    const auto& mask = mComponents.mWorld.mEntityManager.getMask(mComponents.mEntity);
    CUBOS_ASSERT(mId < mask.size(), "Iterator is out of bounds");
    do
    {
        mId += 1;
    } while (mId < mask.size() && !mask.test(mId));

    return *this;
}
