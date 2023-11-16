#pragma once

#include "assimp/material.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "shader.hpp"
#include "render/vertex_array.hpp"
#include "render/vertex_buffer.hpp"
#include "render/index_buffer.hpp"
#include "render/texture.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <memory>

namespace pmodel {

struct Vertex {
    glm::vec3 position;
    glm::vec3 noraml;
    glm::vec3 texCoords;
};

struct TextureInfo {
    unsigned int id;
    std::string type;
};

class Mesh final {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;
    std::unique_ptr<IndexBuffer> ibo_;
public:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture*> textures_;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);

    void Draw(Shader& shader);
private:
    void setupMesh();
};

class Model {
private:
    std::vector<Mesh> meshes_;
    std::string directory_;
public:
    Model(const char* path);
    
    void Draw(Shader& shader);
private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string typeName);
};

} // namespace pmodel
