#pragma once
#include <memory>
#include <string>
#include "shader.hpp"

enum ShaderProgramType {
    Axis,
    Plane,
    Mesh
};

struct ShaderProgram {
    std::string name;
    ShaderProgramType type;
    std::shared_ptr<Shader> shader;
};