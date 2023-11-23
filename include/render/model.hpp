#pragma once

#include <memory>
#include <string>
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "drawable.hpp"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "render/mesh.hpp"
#include "render/model.hpp"
#include "render/shader.hpp"
#include "assimp/material.h"
#include "render/texture.hpp"
#include "render/index_buffer.hpp"
#include "render/vertex_array.hpp"
#include "render/vertex_buffer.hpp"

namespace prism {

class Model final : public prism::Drawable {
private:
    std::vector<TextureInfo> textures_;
    std::vector<std::shared_ptr<Mesh>> meshes_;
    std::string directory_;
    bool flip_;
public:
    Model(const std::string& path, bool flip = true);
    void onRender(Shader& shader) override;
private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

std::shared_ptr<Texture> loadTextureFromFile(const char* path, const std::string& directory, bool flip = true); 

// class Geo {
// public:
//     virtual void Draw(Shader& shader, Texture* tex_diff, Texture* tex_nor) = 0;
// };



// class Plane final : public Geo {
// private:
//     std::unique_ptr<VertexArray> vao_;
//     std::unique_ptr<VertexBuffer> vbo_;
//     std::unique_ptr<IndexBuffer> ibo_;
// public:
//     Plane() {
//         float vertices[] = {
//             -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//              1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
//              1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//             -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//         };
//         unsigned int indices[] = {
//             0, 1, 2,
//             0, 2, 3,
//         };
//         vao_ = std::make_unique<VertexArray>();
//         vao_->Bind();
//         vbo_ = std::make_unique<VertexBuffer>(vertices, sizeof(float) * 4 * 8);
//         ibo_ = std::make_unique<IndexBuffer>(indices, 3 * 2);
//         VertexBufferLayout layout;
//         layout.push_float(3);
//         layout.push_float(3);
//         layout.push_float(2);
//         vao_->addBuffer(*vbo_, layout);
//         vao_->Unbind();
//     }

//     void Draw(Shader& shader, Texture* tex_diff, Texture* tex_nor) override {
//         shader.setUniform1i("tex_diff", 0);
//         tex_diff->Bind(0);
//         shader.setUniform1i("tex_nor", 1);
//         tex_nor->Bind(1);
//         shader.Bind();
//         vao_->Bind();
//         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//         vao_->Unbind();
//         shader.Unbind();
//         glActiveTexture(GL_TEXTURE0);
//     }
// };

} // namespace model