#pragma once

namespace prism {

struct WindowProps {
    const char* title;
    int width;
    int height;
};

static const WindowProps DefaultWindowProps { "Prism Engine", 1920, 1080 };

static const char* FontPath = "D:/home/Prism/resources/font/JetBrainsMonoNerdFontMono-Regular.ttf";

static const char* meshVertexPath = "D:/home/Prism/resources/shader/mesh_vertex.glsl";
static const char* meshFragPath = "D:/home/Prism/resources/shader/mesh_frag.glsl";

} // namespace prism