#version 460 core
layout (location = 0) in vec3 aPosition;

out vec3 TexCoords;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;

void main()
{
    TexCoords = aPosition;
    vec4 pos = proj_matrix * view_matrix * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}