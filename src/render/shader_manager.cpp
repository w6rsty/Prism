#include "render/shader_manager.hpp"
#include "geo/skybox.hpp"

namespace prism {

ShaderManager::ShaderManager(std::vector<CreateShaderInfo> infos)
    : createShaderInfos_(infos) 
{
    CompileShader();
}

ShaderManager::~ShaderManager() {
    createShaderInfos_.clear();
    shaders_.clear();
    namedShaders_.clear();
}

void ShaderManager::CompileShader() {
    for (const auto& info : createShaderInfos_) {
        auto shader = std::make_shared<Shader>(info.vertexPath, info.fragPath);
        if (info.type == ShaderType::SKYBOX) {
            skyboxShader_ = shader;
            continue; // avoid push back to common shaders
        }
        if (info.type == ShaderType::HAS_TEX) {
            hasTexShader_ = shader;
        }
        if (info.type == ShaderType::WITH_TEX) {
            withTexShader_ = shader;
        }
        if (info.type == ShaderType::NAMED) {
            namedShaders_[info.name] = shader;
        }
        shaders_.push_back(shader);
    }
}

std::shared_ptr<Shader> ShaderManager::getPlainShader() const {
    return plainShader_;
}
std::shared_ptr<Shader> ShaderManager::getHasTexShader() const {
    return hasTexShader_;
}
std::shared_ptr<Shader> ShaderManager::getWithTexShader() const {
    return withTexShader_;
}
std::shared_ptr<Shader> ShaderManager::getSkyboxShader() const {
    return skyboxShader_;
}
std::shared_ptr<Shader> ShaderManager::getNamedShader(const std::string& name) const {
    return namedShaders_.at(name);
}

} // namespace prism