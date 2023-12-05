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

class Plane final : public Geo {
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

class Cube final : public Geo {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;
    std::unique_ptr<IndexBuffer> ibo_;

    static float vertices[8 * 36];
public:
    Cube();
    void onRender(Shader& shader) override;
};

class Pyramid final : public Geo {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;

    static float vertices[8 * 18];
public:
    Pyramid();
    void onRender(Shader& shader) override;
};

class Sphere final : public Geo {
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
    ~Sphere();
    void onRender(Shader& shader) override;
};

class Torus final : public Geo {
private:
    std::unique_ptr<VertexArray> vao_;
    std::unique_ptr<VertexBuffer> vbo_;
    std::unique_ptr<IndexBuffer> ibo_;

    int numOfVertices;
    int numOfIndices;
    float* vertices;
    std::vector<int> indices;
public:
    Torus(float precision = 48, float inner = 0.5f, float outer = 1.0f);
    ~Torus();
    void onRender(Shader& shader) override;
};

} // namespace prism