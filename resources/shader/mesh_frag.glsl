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
    vec3 tex_specular = texture(texture_specular1, TexCoord).rgb; // 添加镜面贴图
    vec3 ambeint = 0.2 * tex_diffuse;
    vec3 view_pos = viewPos;
    vec3 norm = normalize(texture(texture_normal1, TexCoord).rgb);
    vec3 lightDir = normalize(light.pos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * tex_diffuse;

    // 计算镜面反射
    vec3 viewDir = normalize(view_pos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // 调整反射光的强度，32.0 是一个参数，你可以根据需要调整

    vec3 specular = light.color * spec * tex_specular; // 乘以镜面贴图
    vec3 result = ambeint + diffuse + specular; // 将镜面反射加入最终结果

    FragColor = vec4(result, 1.0);
}
