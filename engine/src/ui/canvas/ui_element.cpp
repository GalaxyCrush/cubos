#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/ui/canvas/ui_element.hpp>

CUBOS_REFLECT_IMPL(cubos::engine::UIElement)
{
    return core::ecs::TypeBuilder<UIElement>("cubos::engine::UIElement")
        .withField("offset", &UIElement::offset)
        .withField("size", &UIElement::size)
        .withField("pivot", &UIElement::pivot)
        .withField("anchor", &UIElement::anchor)
        .withField("layer", &UIElement::layer)
        .build();
}

glm::vec2 cubos::engine::UIElement::bottomLeftCorner() const
{
    return glm::vec2{position.x - size.x * pivot.x, position.y - size.y * pivot.y};
}

glm::vec2 cubos::engine::UIElement::topRightCorner() const
{
    return glm::vec2{position.x + size.x * (1.0F - pivot.x), position.y + size.y * (1.0F - pivot.y)};
}