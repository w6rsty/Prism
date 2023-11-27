#pragma once

#include "render/drawable.hpp"
#include "render/shader.hpp"
#include "render/index_buffer.hpp"
#include "render/vertex_array.hpp"
#include "render/vertex_buffer.hpp"

class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace prism {

class Geo : public Drawable {
public:
    virtual ~Geo() = default;
    virtual void onRender(Shader& shader) = 0;
};

class Plane final : public Drawable {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;
    std::unique_ptr<IndexBuffer> ibo_;

    static float vertices[4 * 8];
    static unsigned int indices[3 * 2];
public:
    Plane(float scale = 1.0f);
    void onRender(Shader& shader) override;
};

class Cube final : public Drawable {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;
    std::unique_ptr<IndexBuffer> ibo_;

    static float vertices[8 * 36];
public:
    Cube();
    void onRender(Shader& shader) override;
};

class Pyramid final : public Drawable {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;

    static float vertices[8 * 18];
public:
    Pyramid();
    void onRender(Shader& shader) override;
};

class Sphere final : public Drawable {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;
    std::unique_ptr<IndexBuffer> ibo_;

    int numOfVertices;
    int numOfIndices;
    float* vertices;
    std::vector<int> indices;
public:
    Sphere(int precision = 48);
    void onRender(Shader& shader) override;
private:

};

} // namespace prism

// class Torus final {
// private:
//     float _precision;
//     float _innner;
//     float _outer;

//     int numOfVertices;
//     int numOfIndices;
//     float* vertices;
//     std::vector<int> indices;
// public:
//     inline Torus(float precision, float inner, float outer) {
//         init(precision, inner, outer);
//     }
//     inline ~Torus() {
//         delete [] vertices;
//         indices.clear();
//     }
// private:
//     inline void init(float precision, float inner, float outer) {
//         numOfVertices = (precision + 1) * (precision + 1);
//         numOfIndices = precision * precision * 6;
//         indices.reserve(numOfIndices);
//         vertices = new float[numOfVertices * (3 + 2 + 3)];
//         for (int i = 0; i <= precision; i++) {
//             glm::mat4 rMat= glm::rotate(glm::mat4(1.0f), glm::radians(i * 360.0f / precision), glm::vec3(0.0, 0.0, 1.0f));

//         }
//     }
// };