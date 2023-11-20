#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "pecs.hpp"
#include "shader.hpp"
#include "geo/geo.hpp"

#define meshVertexPath "D:/home/Prism/resources/shader/mesh_vertex.glsl"
#define meshFragPath "D:/home/Prism/resources/shader/mesh_frag.glsl"
#define backpackModelPath "D:/home/Prism/resources/model/backpack/backpack.obj"
#define nanosuitModelPath "D:/home/Prism/resources/model/nanosuit/nanosuit.obj"

int main() {
    pecs::World world;

    Renderer rd(1800, 1200, "Prism Engine");
    // set camera
    rd.setCamera(new Camera({0, 1, 1}));
    // set mesh shader
    rd.createShader({
        "mesh",
        meshVertexPath, 
        meshFragPath
    });
    glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
    mat = glm::translate(mat, glm::vec3(0.0, 6.5, 2));
    // set model
    rd.createModel({
        "backpack",
        backpackModelPath,
        false,
        mat
    });

    rd.createModel({
        "nanosuit",
        nanosuitModelPath,
        true,
        glm::scale(glm::mat4(1.0f), glm::vec3(0.6f))
    });

    rd.setGlobalTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));

    if (!rd.init()) {
        return EXIT_FAILURE;
    }
    
    rd.run();

    return EXIT_SUCCESS;
}