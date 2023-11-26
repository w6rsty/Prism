#pragma once

#include "pecs.hpp"
#include "ticker.hpp"
#include "engine/window.hpp"

#define meshVertexPath "D:/home/Prism/resources/shader/mesh_vertex.glsl"
#define meshFragPath "D:/home/Prism/resources/shader/mesh_frag.glsl"
#define backpackModelPath "D:/home/Prism/resources/model/backpack/backpack.obj"
#define nanosuitModelPath "D:/home/Prism/resources/model/nanosuit/nanosuit.obj"
#define roomModelPath "D:/home/Prism/resources/model/room/room.obj"

using namespace pecs;

inline void startupRenderSystem(Commands& command) {
}

inline void updateRenderSystem(Commands& command, Queryer queryer, Resources resources, Events& events) {
    auto& window = resources.Get<prism::Window>();
    window.OnUpdate();
}