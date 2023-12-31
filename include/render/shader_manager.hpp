#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "render/shader.hpp"

namespace prism {

enum class ShaderType {
    PLAIN = 0,
    WITH_TEX,
    HAS_TEX,
    SKYBOX,
    NAMED,
    SHADER_TYPE_COUNT,
};

struct CreateShaderInfo {
    const char* name = "shader";
    const char* vertexPath;
    const char* fragPath;
    ShaderType type;
};

class ShaderManager {
private:
    std::vector<CreateShaderInfo> createShaderInfos_;

    std::vector<std::shared_ptr<Shader>> shaders_;

    std::shared_ptr<Shader> plainShader_;
    std::shared_ptr<Shader> hasTexShader_;
    std::shared_ptr<Shader> withTexShader_;
    std::shared_ptr<Shader> skyboxShader_;
    std::unordered_map<std::string, std::shared_ptr<Shader>> namedShaders_;
public:
    ShaderManager(std::vector<CreateShaderInfo> infos);
    ~ShaderManager();

    std::shared_ptr<Shader> getPlainShader() const;
    std::shared_ptr<Shader> getHasTexShader() const;
    std::shared_ptr<Shader> getWithTexShader() const;
    std::shared_ptr<Shader> getSkyboxShader() const;
    std::shared_ptr<Shader> getNamedShader(const std::string& name) const;
    inline std::vector<std::shared_ptr<Shader>>& getAllShaders() { return shaders_; }
private:
    void CompileShader();
};

} // namespace prism