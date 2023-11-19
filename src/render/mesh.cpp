#include "mesh.hpp"
#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include <memory>
#include <string>

namespace pmodel {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures)
    : vertices_(vertices), indices_(indices), textures_(textures)
{
    setupMesh();
}
void  Mesh::Draw(Shader& shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures_.size(); i++) {
        std::string number;
        std::string name = textures_[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);
        shader.setUniform1i((name + number), i);
        textures_[i].ptr->Bind(i);
    }
    vao_->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0));
    vao_->Unbind();
    GLCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::setupMesh() {
    vao_ = std::make_shared<VertexArray>();
    vbo_ = std::make_shared<VertexBuffer>(&vertices_[0], sizeof(Vertex) * vertices_.size());
    ibo_ = std::make_shared<IndexBuffer>(&indices_[0], indices_.size());

    VertexBufferLayout layout;
    layout.push_float(3);
    layout.push_float(3);
    layout.push_float(2);
    // layout.push_float(3);
    // layout.push_float(3);
    vao_->addBuffer(*vbo_, layout);
    //vao_->Unbind();
}

}