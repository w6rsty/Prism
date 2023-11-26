#include "render/vertex_array.hpp"
#include "render/error.hpp"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &buffer_id));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &buffer_id));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    GLCall(glBindVertexArray(buffer_id));
    vb.Bind();

    size_t offset = 0;
    const auto& elements = layout.getElement();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (void*)offset));
        offset += element.count * VertexBufferElement::getSize(element.type);
    }
    GLCall(glEnableVertexAttribArray(0));
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(buffer_id));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}