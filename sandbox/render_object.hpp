#pragma once

#include "pecs.hpp"
#include "renderer.hpp"

using namespace pecs;

struct RenderObject {
    prism::Renderer* rd;
};


inline void renderSystem(Commands& command, Queryer queryer, Resources resources, Events& events) {
    auto& renderer = resources.Get<RenderObject>();
    renderer.rd->render(); 
}