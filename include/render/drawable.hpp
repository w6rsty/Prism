#pragma once

#include "shader.hpp"

namespace prism {

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void onRender(Shader& shader) = 0;
};

} // namespaec prism