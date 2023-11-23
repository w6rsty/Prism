#version 460 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;
uniform vec3 viewPos;

void main(void) {
    vec3 tex_diffuse = texture(texture_diffuse1, TexCoord).rgb;
    vec3 ambeint = 0.2 * tex_diffuse;
    vec3 view_pos = viewPos;
    vec3 norm = normalize(texture(texture_normal1, TexCoord).rgb);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * tex_diffuse;

    vec3 result = ambeint + diffuse;
    FragColor = vec4(result, 1.0);
}
