#include "geo/geometry.hpp"
#include "render/index_buffer.hpp"

#include <memory>

namespace prism {

float Plane::vertices[4 * 8] = {
    -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

unsigned int Plane::indices[3 * 2] = {
    0, 1, 2,
    0, 2, 3
};

Plane::Plane(float scale) {
    if (scale != 1.0f) {
        for (int i = 0; i < 36; i++) {
            vertices[i * 8 + 6] *= scale;
            vertices[i * 8 + 7] *= scale;
        }
    }
    vao_ = std::make_unique<VertexArray>();
    vao_->Bind();
    vbo_ = std::make_unique<VertexBuffer>(vertices, sizeof(float) * 4 * 8);
    ibo_ = std::make_unique<IndexBuffer>(indices, 3 * 2);
    VertexBufferLayout layout;
    layout.push_float(3);
    layout.push_float(3);
    layout.push_float(2);
    vao_->addBuffer(*vbo_, layout);
    vao_->Unbind();
}

void Plane::onRender(Shader& shader) {
    shader.Bind();
    vao_->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao_->Unbind();
    shader.Unbind();
}

float Cube::vertices[8 * 36] = {
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

     0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
};

Cube::Cube() {
    vao_ = std::make_unique<VertexArray>();
    vao_->Bind();
    vbo_ = std::make_unique<VertexBuffer>(vertices, sizeof(float) * 8 * 36);
    VertexBufferLayout layout;
    layout.push_float(3);
    layout.push_float(3);
    layout.push_float(2);
    vao_->addBuffer(*vbo_, layout);
    vao_->Unbind();
}

void Cube::onRender(Shader& shader) {
    shader.Bind();
    vao_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao_->Unbind();
    shader.Unbind();
}

float Pyramid::vertices[8 * 18] = {
    -0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,
     0.0f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  1.0f,

     0.5f,  0.0f, -0.5f,  0.0f,  1.0f, -1.0f,  1.0f,  0.0f,
    -0.5f,  0.0f, -0.5f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
     0.0f,  0.5f,  0.0f,  0.0f,  1.0f, -1.0f,  0.5f,  1.0f,

     0.5f,  0.0f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.0f, -0.5f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.0f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  1.0f,

    -0.5f,  0.0f,  0.5f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.0f, -0.5f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.0f,  0.5f,  0.0f, -1.0f,  1.0f,  0.0f,  0.5f,  1.0f,

     0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.0f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
};

Pyramid::Pyramid() {
    vao_ = std::make_unique<VertexArray>();
    vao_->Bind();
    vbo_ = std::make_unique<VertexBuffer>(vertices, sizeof(float) * 8 * 36);
    VertexBufferLayout layout;
    layout.push_float(3);
    layout.push_float(3);
    layout.push_float(2);
    vao_->addBuffer(*vbo_, layout);
    vao_->Unbind();
}

void Pyramid::onRender(Shader& shader) {
    shader.Bind();
    vao_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 18);
    vao_->Unbind();
    shader.Unbind();
}

Sphere::Sphere(int precision) {
    numOfVertices = (precision + 1) * (precision + 1);
    numOfIndices = precision * precision * 6;
    indices.reserve(numOfIndices);
    vertices = new float[numOfVertices * (3 + 2 + 3)];

    for (int i = 0; i <= precision; i++) {
        for (int j = 0; j <= precision; j++) {
            float y = (float)cos(glm::radians(180.0f - i * 180.0f / precision));
            float x = -(float)cos(glm::radians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));
            float z = (float)sin(glm::radians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));

            int index = i * (precision + 1) + j;
            vertices[index * 8 + 0] = x; 
            vertices[index * 8 + 1] = y; 
            vertices[index * 8 + 2] = z; 
            vertices[index * 8 + 3] = x; 
            vertices[index * 8 + 4] = y; 
            vertices[index * 8 + 5] = z; 
            vertices[index * 8 + 6] = (float)i / precision; 
            vertices[index * 8 + 7] = (float)j / precision; 
        }
    }

    for (int i = 0; i < precision; i++) {
        for (int j = 0; j < precision; j++) {
            indices[6 * (i * precision + j) + 0] = i * (precision + 1) + j;
            indices[6 * (i * precision + j) + 1] = i * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
            indices[6 * (i * precision + j) + 3] = i * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 4] = (i + 1) * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
        }
    }

    vao_ = std::make_unique<VertexArray>();
    vao_->Bind();
    vbo_ = std::make_unique<VertexBuffer>(vertices, sizeof(float) * 8 * numOfVertices);
    ibo_ = std::make_unique<IndexBuffer>(indices.data(), numOfIndices);
    VertexBufferLayout layout;
    layout.push_float(3);
    layout.push_float(3);
    layout.push_float(2);
    vao_->addBuffer(*vbo_, layout);
    vao_->Unbind();
    delete [] vertices;
    indices.clear();
}

void Sphere::onRender(Shader& shader) {
    shader.Bind();
    vao_->Bind();
        glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0);
    vao_->Unbind();
    shader.Unbind();
}


} // namespace prism