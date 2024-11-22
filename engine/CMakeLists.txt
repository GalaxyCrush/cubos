# engine/CMakeLists.txt
# Cubos engine build configuration

# ---------------------- Declare engine library options -----------------------

option(CUBOS_ENGINE_SAMPLES "Build cubos::engine samples" ${PROJECT_IS_TOP_LEVEL})
option(CUBOS_ENGINE_TESTS "Build cubos::engine tests?" ${PROJECT_IS_TOP_LEVEL})
option(CUBOS_ENGINE_SHARED "Build cubos::engine as shared library?" ON)

message("# Building cubos::engine samples: " ${CUBOS_ENGINE_SAMPLES})
message("# Building cubos::engine tests: " ${CUBOS_ENGINE_TESTS})
message("# Building cubos::engine as shared library: " ${CUBOS_ENGINE_SHARED})

# -------------------- Define engine library source files ---------------------

set(CUBOS_ENGINE_SOURCE
	"src/api.cpp"
	"src/prelude.cpp"

	"src/defaults/plugin.cpp"

	"src/settings/plugin.cpp"
	"src/settings/settings.cpp"

	"src/window/plugin.cpp"

	"src/imgui/plugin.cpp"
	"src/imgui/imgui.cpp"
	"src/imgui/data_inspector.cpp"

	"src/transform/plugin.cpp"
	"src/transform/child_of.cpp"
	"src/transform/local_to_parent.cpp"
	"src/transform/local_to_world.cpp"
	"src/transform/position.cpp"
	"src/transform/rotation.cpp"
	"src/transform/scale.cpp"

	"src/utils/free_camera/plugin.cpp"
	"src/utils/free_camera/controller.cpp"

    "src/audio/audio.cpp"
    "src/audio/bridge.cpp"

	"src/assets/plugin.cpp"
	"src/assets/assets.cpp"
	"src/assets/bridge.cpp"
	"src/assets/asset.cpp"
	"src/assets/meta.cpp"
	"src/assets/bridges/file.cpp"

	"src/gizmos/plugin.cpp"
	"src/gizmos/gizmos.cpp"
	"src/gizmos/renderer.cpp"
	"src/gizmos/target.cpp"

	"src/image/bridge.cpp"
	"src/image/image.cpp"
	"src/image/plugin.cpp"

	"src/scene/plugin.cpp"
	"src/scene/scene.cpp"
	"src/scene/bridge.cpp"

	"src/voxels/plugin.cpp"
	"src/voxels/grid.cpp"
	"src/voxels/grid_bridge.cpp"
	"src/voxels/material.cpp"
	"src/voxels/palette.cpp"
	"src/voxels/palette_bridge.cpp"
	"src/voxels/model_bridge.cpp"
	"src/voxels/model.cpp"

	"src/collisions/plugin.cpp"
	"src/collisions/interface/plugin.cpp"
	"src/collisions/interface/collider.cpp"
	"src/collisions/interface/colliding_with.cpp"
	"src/collisions/interface/contact_manifold.cpp"
	"src/collisions/interface/shapes/box.cpp"
	"src/collisions/interface/raycast.cpp"
	"src/collisions/interface/shapes/capsule.cpp"
	"src/collisions/interface/shapes/voxel.cpp"
	"src/collisions/broad_phase/plugin.cpp"
	"src/collisions/broad_phase/sweep_and_prune.cpp"
	"src/collisions/broad_phase/potentially_colliding_with.cpp"
	"src/collisions/narrow_phase/plugin.cpp"

	"src/physics/plugin.cpp"
	"src/physics/gravity.cpp"
	"src/physics/solver/plugin.cpp"
	"src/physics/constraints/penetration_constraint.cpp"
	"src/physics/solver/penetration_constraint/plugin.cpp"
	"src/physics/fixed_substep/plugin.cpp"
	"src/physics/fixed_substep/substeps.cpp"
	"src/physics/solver/integration/plugin.cpp"

	"src/input/plugin.cpp"
	"src/input/input.cpp"
	"src/input/combination.cpp"
	"src/input/bindings.cpp"
	"src/input/action.cpp"
	"src/input/axis.cpp"

	"src/ui/canvas/plugin.cpp"
	"src/ui/canvas/canvas.cpp"
	"src/ui/canvas/draw_list.cpp"
	"src/ui/canvas/keep_pixel_size.cpp"
	"src/ui/canvas/native_aspect_ratio.cpp"
	"src/ui/canvas/match_height.cpp"
	"src/ui/canvas/match_width.cpp"
	"src/ui/canvas/element.cpp"
	"src/ui/canvas/expand.cpp"
	"src/ui/canvas/vertical_stretch.cpp"
	"src/ui/canvas/horizontal_stretch.cpp"
	"src/ui/color_rect/plugin.cpp"
	"src/ui/color_rect/color_rect.cpp"
	"src/ui/image/plugin.cpp"
	"src/ui/image/image.cpp"

	"src/fixed_step/plugin.cpp"
	"src/fixed_step/fixed_delta_time.cpp"

	"src/render/defaults/plugin.cpp"
	"src/render/defaults/target.cpp"
	"src/render/shader/plugin.cpp"
	"src/render/shader/shader.cpp"
	"src/render/shader/bridge.cpp"
	"src/render/g_buffer/plugin.cpp"
	"src/render/g_buffer/g_buffer.cpp"
	"src/render/hdr/plugin.cpp"
	"src/render/hdr/hdr.cpp"
	"src/render/target/plugin.cpp"
	"src/render/target/target.cpp"
	"src/render/picker/plugin.cpp"
	"src/render/picker/picker.cpp"
	"src/render/depth/plugin.cpp"
	"src/render/depth/depth.cpp"
	"src/render/camera/plugin.cpp"
	"src/render/camera/orthographic.cpp"
	"src/render/camera/perspective.cpp"
	"src/render/camera/draws_to.cpp"
	"src/render/camera/camera.cpp"
	"src/render/voxels/plugin.cpp"
	"src/render/voxels/load.cpp"
	"src/render/voxels/grid.cpp"
	"src/render/voxels/palette.cpp"
	"src/render/tone_mapping/plugin.cpp"
	"src/render/tone_mapping/tone_mapping.cpp"
	"src/render/lights/plugin.cpp"
	"src/render/lights/environment.cpp"
	"src/render/lights/directional.cpp"
	"src/render/lights/point.cpp"
	"src/render/lights/spot.cpp"
	"src/render/mesh/plugin.cpp"
	"src/render/mesh/vertex.cpp"
	"src/render/mesh/pool.cpp"
	"src/render/mesh/mesh.cpp"
	"src/render/g_buffer_rasterizer/plugin.cpp"
	"src/render/g_buffer_rasterizer/g_buffer_rasterizer.cpp"
	"src/render/ssao/plugin.cpp"
	"src/render/ssao/ssao.cpp"
	"src/render/deferred_shading/plugin.cpp"
	"src/render/deferred_shading/deferred_shading.cpp"
	"src/render/split_screen/plugin.cpp"
	"src/render/split_screen/split_screen.cpp"
	"src/render/bloom/plugin.cpp"
	"src/render/bloom/bloom.cpp"
	"src/render/shadows/plugin.cpp"
	"src/render/shadows/caster.cpp"
	"src/render/shadows/spot_caster.cpp"
	"src/render/shadows/directional_caster.cpp"
	"src/render/shadows/point_caster.cpp"
	"src/render/shadow_atlas/plugin.cpp"
	"src/render/shadow_atlas/shadow_atlas.cpp"
	"src/render/shadow_atlas_rasterizer/plugin.cpp"
	"src/render/shadow_atlas_rasterizer/shadow_atlas_rasterizer.cpp"
	"src/render/cascaded_shadow_maps/plugin.cpp"
	"src/render/cascaded_shadow_maps_rasterizer/plugin.cpp"

	"src/tools/settings_inspector/plugin.cpp"
	"src/tools/selection/plugin.cpp"
	"src/tools/selection/selection.cpp"
	"src/tools/world_inspector/plugin.cpp"
	"src/tools/entity_inspector/plugin.cpp"
	"src/tools/debug_camera/plugin.cpp"
	"src/tools/toolbox/plugin.cpp"
	"src/tools/toolbox/toolbox.cpp"
	"src/tools/transform_gizmo/plugin.cpp"
	"src/tools/metrics_panel/plugin.cpp"
	"src/tools/collider_gizmos/plugin.cpp"
	"src/tools/play_pause/plugin.cpp"
	"src/tools/ecs_statistics/plugin.cpp"
	"src/tools/console/plugin.cpp"
	"src/tools/plugin.cpp"
)

# ---------------------- Configure engine library target ----------------------

if(CUBOS_ENGINE_SHARED)
	add_library(cubos-engine SHARED ${CUBOS_ENGINE_SOURCE})
	target_compile_definitions(cubos-engine
		PRIVATE -DCUBOS_ENGINE_EXPORT # When compiling library source files, we export symbols
		PUBLIC -DCUBOS_ENGINE_IMPORT  # When compiling client source files, we import symbols
	)
else()
	add_library(cubos-engine STATIC ${CUBOS_ENGINE_SOURCE})
endif()
add_library(cubos::engine ALIAS cubos-engine) # Alias so that we can use cubos::engine inside the build tree
set_target_properties(cubos-engine PROPERTIES EXPORT_NAME engine) # Will be within the cubos namespace when exported
cubos_common_target_options(cubos-engine)

target_include_directories(cubos-engine PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include/cubos-engine> # On installation, the include files will have been copied here 
)

# Set the default path for the builtin assets folder, which is useful during development.
set(CUBOS_ENGINE_ASSETS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(CUBOS_ENGINE_ASSETS_PATH ${CUBOS_ENGINE_ASSETS_PATH} PARENT_SCOPE)
target_compile_definitions(cubos-engine PRIVATE CUBOS_ENGINE_ASSETS_PATH="${CUBOS_ENGINE_ASSETS_PATH}")

# ------------- Fetch and link engine private library dependencies -------------

include(FetchContent)

# Fetch stb_image
FetchContent_Declare(
	stb_image
	GIT_REPOSITORY https://github.com/GameDevTecnico/cubos-stb.git
	GIT_TAG        5c340b5ee24ac74e69bf92b5bacdfbac6bbaa4a8
	SYSTEM
)
FetchContent_MakeAvailable(stb_image)

# We need to define our own stb_image target as the repository doesn't have its own CMakeLists.txt
add_library(stb_image INTERFACE)
target_include_directories(stb_image SYSTEM INTERFACE ${stb_image_SOURCE_DIR})

# Link stb_image
target_link_libraries(cubos-engine PRIVATE $<BUILD_INTERFACE:stb_image>)

# ------------- Fetch and link engine public library dependencies -------------

# Link our own core library
target_link_libraries(cubos-engine PUBLIC cubos-core)

# Fetch ImGui and ImPlot
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG        v1.89.9
)
FetchContent_Declare(
	implot
	GIT_REPOSITORY https://github.com/epezent/implot.git
	GIT_TAG        1f7a8c0314d838a76695bccebe0f66864f507bc0
)
FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(implot)

# We need to define our own ImGui target as the ImGui repository doesn't have its own CMakeLists.txt
set(IMGUI_SOURCES
	"${imgui_SOURCE_DIR}/imgui.cpp"
	"${imgui_SOURCE_DIR}/imgui_draw.cpp"
	"${imgui_SOURCE_DIR}/imgui_tables.cpp"
	"${imgui_SOURCE_DIR}/imgui_widgets.cpp"
	"${imgui_SOURCE_DIR}/imgui_demo.cpp"
	"${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp"

	"${implot_SOURCE_DIR}/implot.cpp"
	"${implot_SOURCE_DIR}/implot_items.cpp"
	"${implot_SOURCE_DIR}/implot_demo.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(imgui PUBLIC
	$<BUILD_INTERFACE:${imgui_SOURCE_DIR}>
	$<BUILD_INTERFACE:${imgui_SOURCE_DIR}/misc/cpp>
	$<BUILD_INTERFACE:${implot_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include/imgui> # On installation, we copy the include files here
	$<INSTALL_INTERFACE:include/imgui/misc/cpp>
	$<INSTALL_INTERFACE:include/implot>
)

# Finally, link the target we created for both ImGui and Implot
target_link_libraries(cubos-engine PUBLIC imgui)

# ------------------------ Configure tests and samples ------------------------

if(CUBOS_ENGINE_TESTS)
	add_subdirectory(tests)
endif()

if(CUBOS_ENGINE_SAMPLES)
	add_subdirectory(samples)
endif()

# ------------------- Configure engine library installation -------------------

if(CUBOS_ENABLE_INSTALL)
    set(CUBOS_ENGINE_ASSETS_INSTALL_PATH ${CMAKE_INSTALL_DATADIR}/cubos-engine)
    set(CUBOS_ENGINE_ASSETS_INSTALL_PATH ${CUBOS_ENGINE_ASSETS_INSTALL_PATH} PARENT_SCOPE)

	# Install engine target and imgui target
    install(TARGETS cubos-engine imgui
        EXPORT cubos-engine-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cubos-engine)
	install(DIRECTORY ${imgui_SOURCE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui)
	install(DIRECTORY ${implot_SOURCE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/implot)
	install(DIRECTORY assets/ DESTINATION ${CUBOS_ENGINE_ASSETS_INSTALL_PATH})
endif()
