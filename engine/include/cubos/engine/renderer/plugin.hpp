#pragma once

#include <cubos/core/gl/grid.hpp>
#include <cubos/core/gl/palette.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/renderer/renderer.hpp>

namespace cubos::engine
{
    /// Component which makes a voxel grid be rendered by the renderer plugin.
    /// Must be used with `LocalToWorld`, which provides the transformation matrix of the grid.
    struct [[cubos::component("cubos/renderable_grid", VecStorage)]] RenderableGrid
    {
        Asset<core::gl::Grid> asset;                     ///< Handle to the grid asset to be rendered.
        glm::vec3 offset = {0.0F, 0.0F, 0.0F};           ///< Translation applied to the voxel grid before any other.
        [[cubos::ignore]] RendererGrid handle = nullptr; ///< Handle to the uploaded grid - set automatically.
    };

    /// Component which defines parameters of a camera used to render the world.
    struct [[cubos::component("cubos/camera", VecStorage)]] Camera
    {
        float fovY;  ///< The vertical field of view in degrees.
        float zNear; ///< The near clipping plane.
        float zFar;  ///< The far clipping plane.
    };

    /// @brief Resource which identifies the camera entities to be used by the renderer.
    struct ActiveCameras
    {
        /// @brief Entities which represent the current active cameras. If more than one is set,
        /// the screen is split. At most, 4 cameras are supported at the same time.
        core::ecs::Entity entities[4];
    };

    /// Plugin to create and handle the lifecycle of a renderer. It renders all entities with a
    /// `RenderableGrid` and `LocalToWorld` components.
    ///
    /// @details This plugin adds three systems: one to initialize the renderer, one to collect the
    /// frame information and one to draw the frame.
    ///
    /// Dependencies:
    /// - `windowPlugin`
    /// - `transformPlugin`
    /// - `assetsPlugin`
    ///
    /// Resources:
    /// - `Renderer`: handle to the renderer.
    /// - `RendererFrame`: holds the current frame information.
    /// - `RendererEnvironment`: holds the environment information (ambient light, sky gradient).
    /// - `ActiveCameras`: the entities which represents the active cameras.
    ///
    /// Components:
    /// - `RenderableGrid`: a grid to be rendered. Must be used with `LocalToWorld`.
    /// - `Camera`: holds camera information. The entity pointed to by `ActiveCameras` must have
    ///   this component.
    /// - `SpotLight`: the entity emits a spot light.
    /// - `DirectionalLight`: the entity emits a directional light.
    /// - `PointLight`: the entity emits a point light.
    ///
    /// Startup tags:
    /// - `cubos.renderer.init`: initializes the renderer, after `cubos.window.init`.
    /// - `cubos.renderer.ssao.enabled`: whether SSAO is enabled.
    ///
    /// Tags:
    /// - `cubos.renderer.frame`: collects the frame information, after `cubos.transform.update`.
    /// - `cubos.renderer.draw`: renders the frame to the window, after `cubos.renderer.frame`
    ///   and before `cubos.window.render`.
    ///
    /// @param cubos CUBOS. main class
    void rendererPlugin(Cubos& cubos);
} // namespace cubos::engine
