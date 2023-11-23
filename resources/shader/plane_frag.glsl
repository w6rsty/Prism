#version 460 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D tex_diff;
uniform sampler2D tex_nor;

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;
uniform vec3 viewPos;

void main(void) {
    vec3 tex_diffuse = texture(tex_diff, TexCoord).rgb;
    vec3 ambient = 0.2 * tex_diffuse;
    vec3 view_pos = viewPos;    
    vec3 norm = normalize(texture(tex_nor, TexCoord).rgb);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * tex_diffuse;

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
