#pragma once

#include "pecs.hpp"
#include "shader.hpp"
#include "glm/glm.hpp"
#include "drawable.hpp"

namespace prism {

struct DrawInfo {
    pecs::Entity entity;
    Shader& shader;
    glm::mat4 mMat = glm::mat4(1.0f);
};

class DrawableManager final {
private:

public:
};

} // namespace prism