#pragma once

#include "glm/glm.hpp"
#include <memory>
#include "index_buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include <string>
#include <vector>

namespace pmodel {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    // glm::vec3 tangent;
    // glm::vec3 bitTangent;
};

struct TextureInfo {
    std::shared_ptr<Texture> ptr;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<TextureInfo> textures_;
    std::shared_ptr<VertexArray> vao_;
private:
    std::shared_ptr<VertexBuffer> vbo_;
    std::shared_ptr<IndexBuffer> ibo_;
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures);
    void Draw(Shader& shader);
private:
    void setupMesh();
};

}