#include "model.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "pecs.hpp"
#include "shader.hpp"
#include <memory>

#define meshVertexPath "../resources/shader/mesh_vertex.glsl"
#define meshFragPath "../resources/shader/mesh_frag.glsl"

int main() {
    pecs::World world;

    Renderer rd(1800, 1200, "Prism Engine");
    // set camera
    rd.setCamera(new Camera());
    // set mesh shader
    rd.createShader({
        "mesh",
        meshVertexPath, 
        meshFragPath
    });
    // set model
    rd.createModel({
        "backpack",
        "../resources/model/backpack/backpack.obj",
        false
    });

    rd.createModel({
        "nanosuit",
        "../resources/model/nanosuit/nanosuit.obj",
        false
    });

    if (!rd.init()) {
        return EXIT_FAILURE;
    }
    
    rd.run();

    return EXIT_SUCCESS;
}