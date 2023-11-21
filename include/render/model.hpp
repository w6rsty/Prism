#pragma once
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "mesh.hpp"
#include "render/mesh.hpp"
#include "render/model.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <iterator>
#include <memory>
#include <string>

namespace pmodel {

class Model {
public:
    std::vector<TextureInfo> textures_;
    std::vector<std::shared_ptr<Mesh>> meshes_;
    std::string directory_;
private:
    bool flip_;
public:
    Model(const std::string& path, bool flip = true);
    void Draw(Shader& shader);
private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

std::shared_ptr<Texture> loadTextureFromFile(const char* path, const std::string& directory, bool flip = true); 

class Geo {
public:
    virtual void Draw(Shader& shader, Texture* tex_diff, Texture* tex_nor) = 0;
};



class Plane final : public Geo {
private:
    unsigned int vao_, vbo_, ibo_;
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };
    std::vector<Vertex> vertices_;
public:
    Plane() {
        vertices_.push_back({{-1,  0,  1}, {0, 1, 0}, {0, 0}});
        vertices_.push_back({{ 1,  0,  1}, {0, 1, 0}, {20, 0}});
        vertices_.push_back({{ 1,  0, -1}, {0, 1, 0}, {20, 20}});
        vertices_.push_back({{-1,  0, -1}, {0, 1, 0}, {0, 20}});
        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,
        };
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ibo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size() , vertices_.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 2, indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glBindVertexArray(0);
    }
    ~Plane() {

    }
    void Draw(Shader& shader, Texture* tex_diff, Texture* tex_nor) override {
        shader.setUniform1i("tex_diff", 0);
        tex_diff->Bind(0);
        shader.setUniform1i("tex_nor", 1);
        tex_nor->Bind(1);
        shader.Bind();
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        shader.Unbind();
        glActiveTexture(GL_TEXTURE0);
    }
};

}