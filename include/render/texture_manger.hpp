#pragma once

#include <memory>
#include <string>
#include <vector>
#include "pecs.hpp"
#include <algorithm>
#include "texture.hpp"
#include "unordered_map"

namespace prism {

class TextureManager final {
private:
    using TexturePtr = std::shared_ptr<Texture>;
    std::unordered_map<std::string, TexturePtr> textures;
public:
    void Add(const char* name, TexturePtr tex);
    std::shared_ptr<Texture> Get(const char* name) const;
private:
    template <typename... Names>
    std::vector<TexturePtr> doGet(Names&&... names, std::vector<TexturePtr>& result);
};

} // namespace prism