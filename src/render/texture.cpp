#include "render/texture.hpp"
#include "render/error.hpp"

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
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        stbi_image_free(data);
    }
}

Texture::Texture(int width, int height, const unsigned char* data) {
    GLCall(glGenTextures(1, &texture_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));

    if (data) {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &texture_id));
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));
}
void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glActiveTexture(GL_TEXTURE0));
}

SkyBoxTexture::SkyBoxTexture(std::vector<std::string>& faces, bool flip) {
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        stbi_set_flip_vertically_on_load(flip);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
void SkyBoxTexture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id));
}
void SkyBoxTexture::Unbind() const  {
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    GLCall(glActiveTexture(GL_TEXTURE0));
}

SkyBoxTexture::~SkyBoxTexture() {
    glDeleteTextures(1, &texture_id);
}