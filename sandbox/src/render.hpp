#pragma once

#include "pecs.hpp"
#include "engine/renderer.hpp"
#include "config/config.hpp"
#include "ticker.hpp"

#define meshVertexPath "D:/home/Prism/resources/shader/mesh_vertex.glsl"
#define meshFragPath "D:/home/Prism/resources/shader/mesh_frag.glsl"
#define backpackModelPath "D:/home/Prism/resources/model/backpack/backpack.obj"
#define nanosuitModelPath "D:/home/Prism/resources/model/nanosuit/nanosuit.obj"
#define roomModelPath "D:/home/Prism/resources/model/room/room.obj"

using namespace pecs;

struct RenderObject {
    prism::Renderer* rd;  
};

inline void startupRenderSystem(Commands& command) {
    prism::Renderer* rd = new prism::Renderer(WIDTH, HEIGHT, GAME_NAME);
    GAME_WINDOW = glfwCreateWindow(WIDTH, HEIGHT, GAME_NAME, NULL, NULL);
    rd->setWindow(GAME_WINDOW);
    // set camera
    rd->setCamera(new Camera({0, 0, 1}));
    // set mesh shader
    if (!rd->init()) {
        assert(false);
    }

    command.SetResource(RenderObject{ .rd = rd });
}

inline void renderSystem(Commands& command, Queryer queryer, Resources resources, Events& events) {
    auto& renderer = resources.Get<RenderObject>();
    renderer.rd->render();
}