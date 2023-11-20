#version 460 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main(void) {
    vec3 texture_diffuse = texture(texture_diffuse1, TexCoord).rgb;
    vec3 ambeint = 0.2 * texture_diffuse;

    vec3 norm = texture(texture_normal1, TexCoord).rgb;
    vec3 lightDir = normalize(vec3(2.0, 2.0, 1.0) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture_diffuse;

    vec3 result = ambeint + diffuse;
    FragColor = vec4(result, 1.0);
}
