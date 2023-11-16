#include "render/mesh.hpp"


#include <string>

namespace pmodel {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures)
    : vertices_(vertices), indices_(indices), textures_(textures)
{
    setupMesh();
}

void Mesh::setupMesh() {
    vao_ = std::make_unique<VertexArray>(VertexArray{});
    vbo_ = std::make_unique<VertexBuffer>(VertexBuffer(&vertices_[0], sizeof(Vertex) * vertices_.size()));
    ibo_ = std::make_unique<IndexBuffer>(IndexBuffer(&indices_[0], indices_.size()));

    VertexBufferLayout layout;
    layout.push_float(3);
    layout.push_float(3);
    layout.push_float(2);
    vao_->addBuffer(*vbo_, layout);
}

void Mesh::Draw(Shader& shader) {
    for (int i = 0; i < textures_.size(); i++) {
        textures_[i]->Bind();
        shader.setUniform1i(std::to_string(i), i);
    }
    vao_->Bind();
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    vao_->Unbind();
}

void Model::Draw(Shader& shader) {
    for (unsigned int i = 0; i < meshes_.size(); i++) {
        meshes_[i].Draw(shader);
    }
}

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "[Assimp Error] " << import.GetErrorString() << std::endl;
    }
    directory_ = path.substr(0, path.find_last_of("/"));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes_.push_back(processMesh(mesh, scene));         
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

} // namespace pmodel