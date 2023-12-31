#version 460 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D tex;

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;
uniform vec3 viewPos;

void main(void) {
    vec3 texColor = texture(tex, TexCoord).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * texColor;

    // Calculate specular reflection
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);

    // Calculate specular component
    vec3 specular = light.color * spec;

    vec3 ambient = light.color * texColor;

    vec3 result = (0.4 * ambient + 0.8 * diffuse + 0.15 * specular) * texColor;
    FragColor = vec4(result, 1.0);
}