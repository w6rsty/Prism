#include "pecs.hpp"
#include "systems.hpp"

using namespace pecs;

int main() {
    pecs::World world;

    world.AddStartupSystem(startupTickerSystem)
         .AddStartupSystem(startupRenderSystem)
         .AddSystem(renderSystem)
         .AddSystem(updateTickerSystem)
         .AddSystem(echoTickerSystem)
         ;

    world.Startup();
    
    while (true) {
        world.Update();
    }

    world.Shutdown();
    return EXIT_SUCCESS;
}