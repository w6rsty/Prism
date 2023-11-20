#version 460 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;


void main(void) {
    vec3 texture_diffuse = texture(texture_diffuse1, TexCoord).rgb;

    vec3 result = texture_diffuse;
    FragColor = vec4(result, 1.0);
}
