#pragma once
#include "glad/glad.h"
#include "error.hpp"
#include <cstdlib>
#include <iostream>
#include <cassert>

#define ASSERT(x) if (!(x)) assert(x);

#define GLCall(x) x; do { \
    GLClearError(); \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)); \
} while (0)

inline void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glad_glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

inline void printShaderLog(unsigned int shader) {
    int len = 0;
    int chWritten = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &chWritten, log);
        std::cout << "\x1b[33;1m[Shader Error]\x1b[0m: " << log << std::endl;
        free(log);
    }
}

inline void printProgramLog(unsigned int program) {
    int len = 0;
    int chWritten = 0;
    char *log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *)malloc(len);
        glGetProgramInfoLog(program, len, &chWritten, log);
        std::cout << "\x1b[33;1m[Program Error]\x1b[0m: " << log << std::endl;
        free(log);
    }
}

inline bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        std::cout << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}