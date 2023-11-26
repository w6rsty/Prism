#include "render/shader_manager.hpp"

namespace prism {

ShaderManager::ShaderManager(std::vector<CreateShaderInfo> infos)
    : createShaderInfos_(infos) 
{

}

ShaderManager::~ShaderManager() {
    createShaderInfos_.clear();
    shaders_.clear();
    toolShaders_.clear();
    specialShaders_.clear();
}

void ShaderManager::CompileShader() {
    for (const auto& info : createShaderInfos_) {
        auto shader = std::make_shared<Shader>(info.vertexPath, info.fragPath);
        shaders_.push_back(shader);

        switch (info.type) {
            case ShaderType::TOOL:
                toolShaders_[info.name] = shader;
                break;
            case ShaderType::SPECIAL:
                specialShaders_[info.name] = shader;
                break;
            default:
                break;
        }
    }
}

std::shared_ptr<Shader> ShaderManager::getPlainShader() const {
    return plainShader_;
}
std::shared_ptr<Shader> ShaderManager::getHasTexShader() const {
    return hasTexShader_;
}
std::shared_ptr<Shader> ShaderManager::getWidthTexShader() const {
    return withTexShader_;
}
const std::unordered_map<std::string, std::shared_ptr<Shader>>& ShaderManager::getToolShaders() const {
    return toolShaders_;
}
const std::unordered_map<std::string, std::shared_ptr<Shader>>& ShaderManager::getSpecialShaders() const {
    return specialShaders_;
}

} // namespace prism