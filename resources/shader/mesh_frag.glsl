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
    vec3 texture_specular = texture(texture_specular1, TexCoord).rgb;
    vec3 texture_normal = texture(texture_normal1, TexCoord).rgb;
    
    vec3 ambeint = texture_diffuse; 
    
    vec3 result = ambeint + texture_specular * 0.2 + texture_normal * 0.2;
    FragColor = vec4(result, 1.0);
}
