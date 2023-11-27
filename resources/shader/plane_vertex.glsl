#version 460 core

layout (location = 0) in vec3 aLocation;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model_matrix;
uniform mat4 proj_matrix;
uniform mat4 view_matrix;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main(void)
{
    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(aLocation, 1.0);
    FragPos = (model_matrix * vec4(aLocation, 1.0)).xyz;
    Normal = mat3(transpose(inverse(model_matrix))) * aNormal;
    TexCoord = aTexCoord;
}
