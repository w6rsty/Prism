#include "renderer.hpp"
#include "render/mesh.hpp"

int main() {

    Renderer rd(1800, 1200, "Prism Engine");
    if (!rd.init()) {
        return EXIT_FAILURE;
    }
    
    rd.run();

    return EXIT_SUCCESS;
}