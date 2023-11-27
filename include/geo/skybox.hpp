#pragma once

#include "render/drawable.hpp"
#include "render/vertex_array.hpp"
#include "render/vertex_buffer.hpp"
#include "render/index_buffer.hpp"

namespace prism {

class SkyBox final : public Drawable {
private:
    unsigned int loadCubemap(std::vector<std::string>& faces);
    
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;

    unsigned int texture_id_;
    
    static float vertices[108];
public:
    SkyBox(std::vector<std::string>& faces);
    void onRender(Shader& shader) override;
};

} // namespace prism