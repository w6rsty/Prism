#version 460 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D samp;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void) {
    vec4 tex_color = texture(samp, TexCoords);
    float ambientStrength = 0.2;
    vec3 ambeint = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambeint + diffuse) * tex_color.xyz;
    FragColor = vec4(result, 1.0);
}
