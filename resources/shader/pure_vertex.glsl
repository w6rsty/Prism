#version 460 core

layout (location = 0) in vec3 aLocation;

uniform mat4 model_matrix;
uniform mat4 proj_matrix;
uniform mat4 view_matrix;

void main(void)
{
    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(aLocation, 1.0);
}
