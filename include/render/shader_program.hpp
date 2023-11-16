#pragma once
#include <string>
#include "shader.hpp"

enum ShaderProgramType {
    Axis,
    Pure,
    Tex_Normal,
    Color,
    Mesh
};

struct ShaderProgram {
    std::string name;
    ShaderProgramType type;
    Shader* shader;
};