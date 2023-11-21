#include "render/index_buffer.hpp"
#include "render/error.hpp"

IndexBuffer::IndexBuffer(const void* data, unsigned int count) {
    GLCall(glGenBuffers(1, &buffer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::update(const void* data, unsigned int count) {
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &buffer_id));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id));
}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}