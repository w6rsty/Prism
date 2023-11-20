#pragma once
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
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

}

namespace mmodel {

class Model {
public:
    Model(const std::string& path);
    void Draw(Shader& shader);
private:
    void loadModel(const std::string& path);
};

}