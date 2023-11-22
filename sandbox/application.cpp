#include "pecs.hpp"
#include "startup.hpp"
#include "glm/glm.hpp"
#include "renderer.hpp"
#include "render/model.hpp"
#include "render/camera.hpp"
#include "render/shader.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace pecs;

int main() {
    pecs::World world;

    world.AddStartupSystem(startup)
         .AddSystem(renderSystem)
         .AddSystem(updateTickerSystem)
         .AddSystem(echoTickerSystem);

    world.Startup();

    while (true) {
        world.Update();
    }

    world.Shutdown();
    return EXIT_SUCCESS;
}