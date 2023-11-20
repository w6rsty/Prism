#include "shader.hpp"

#include "error.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vs, const char* fs)
    : renderer_id(0) 
{
    file_paths[0] = vs;
    file_paths[1] = fs;
    renderer_id = createShader();
}

std::string Shader::parseShader(const std::string& filePath) {
    std::ifstream stream(filePath, std::ios::in);
    std::string line = "";
    std::stringstream ss;
    while (getline(stream, line)) {
        ss << line << "\n";
    }
    return ss.str();
}

unsigned int Shader::compileShader(unsigned int type, const char* source, ShaderType s_type) {
    unsigned int id;
    id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    checkOpenGLError();
    int length;
    glGetShaderiv(id, GL_COMPILE_STATUS, &length);
    if (length != 1) {
        if (s_type == ShaderType::Vertex)
            std::cout << "\x1b[31;1mVertex Shader compilation failed\x1b[0m" << std::endl;
        else
            std::cout << "\x1b[31;1mFragment Shader compilation failed\x1b[0m" << std::endl;
        printShaderLog(id);
    }
    return id;
}

unsigned int Shader::createShader() {
    unsigned int vs, fs;
    std::string vsSrc = parseShader(file_paths[0]);
    std::string fsSrc = parseShader(file_paths[1]);
    vs = compileShader(GL_VERTEX_SHADER, vsSrc.c_str(), Vertex);
    fs  = compileShader(GL_FRAGMENT_SHADER, fsSrc.c_str(), Fragment);
    
    unsigned int program;
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    checkOpenGLError();
    int length;
    glGetProgramiv(program, GL_LINK_STATUS, &length);
    if (length != 1) {
        std::cout << "\x1b[31;1mShader Linking failed\x1b[0m" << std::endl;
        printProgramLog(program);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::program() {
    return renderer_id;
}

void Shader::Bind() const {
    GLCall(glUseProgram(renderer_id));
}
void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

Shader::~Shader() {
    GLCall(glDeleteProgram(renderer_id));
}

int Shader::getUniformLocation(const std::string& name) {
    if (uniform_cache.find(name) != uniform_cache.end()) {
        return uniform_cache[name];
    }
    int location;
    location = glGetUniformLocation(renderer_id, name.c_str());
    if (location == -1) {
        std::cout << "Warning: no uniform named: `" << name << "`\n";
    }
    uniform_cache[name] = location;
    return location;
}

void Shader::setUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform2f(const std::string& name, float v0, float v1) {
    GLCall(glUniform2f(getUniformLocation(name), v0, v1));
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
    GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
}
void Shader::setUniformMat4f(const std::string& name, glm::mat4& mat) {
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}