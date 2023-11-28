#pragma once

namespace prism {

struct WindowProps {
    const char* title;
    int width;
    int height;
};

static const int WIDTH = 1920;
static const int HEIGHT = 1080;
static const float ASPECT = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);

static const WindowProps DefaultWindowProps { "Prism Engine", WIDTH, HEIGHT }; 

static const char* FontPath = "D:/home/Prism/resources/font/JetBrainsMonoNerdFontMono-Regular.ttf";

// Shader path
static const char* meshVertexPath = "D:/home/Prism/resources/shader/mesh_vertex.glsl";
static const char* meshFragPath = "D:/home/Prism/resources/shader/mesh_frag.glsl";

static const char* withTexVertexPath = "D:/home/Prism/resources/shader/with_tex_vertex.glsl";
static const char* withTexFragPath = "D:/home/Prism/resources/shader/with_tex_frag.glsl";

static const char* skyboxVertexPath = "D:/home/Prism/resources/shader/sky_vertex.glsl";
static const char* skyboxFragPath = "D:/home/Prism/resources/shader/sky_frag.glsl";

} // namespace prism