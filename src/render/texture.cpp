#include "texture.hpp"
#include "error.hpp"

Texture::Texture(const std::string& filePath, bool flip) 
    : file_path(filePath), data(nullptr), width(0), height(0), BPP(0), flip_(flip)
{
    GLCall(glGenTextures(1, &texture_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
    stbi_set_flip_vertically_on_load(flip_);
    data = stbi_load(filePath.c_str(), &width, &height, &BPP, 0);
    if (data) {
        GLenum format;
        if (BPP == 1) {
            format = GL_RED;
        } else if (BPP == 3) {
            format = GL_RGB;
        } else if (BPP == 4) {
            format = GL_RGBA;
        }
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
}

Texture::Texture(int width, int height, const unsigned char* data) {
    GLCall(glGenTextures(1, &texture_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));

    if (data) {
        GLenum format;
        if (BPP == 1) {
            format = GL_RED;
        } else if (BPP == 3) {
            format = GL_RGB;
        } else if (BPP == 4) {
            format = GL_RGBA;
        }
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
}

Texture::~Texture() {
    if (data) stbi_image_free(data);
    GLCall(glDeleteTextures(1, &texture_id));
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
}
void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}