#pragma once

#include "engine/prism.hpp"

struct RenderInfo {
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    glm::mat4* model = nullptr;

    std::shared_ptr<prism::Drawable> drawable;
    prism::ShaderType shaderType;
    
    // Only for WITH_TEX use
    unsigned int texIndex = 0;
};