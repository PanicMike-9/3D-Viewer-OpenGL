#version 410 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

uniform sampler2D texture_diffuse1;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};

struct Light 
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    vec3 model_color = texture(texture_diffuse1, TexCoords).rgb;

    float ambient_strength = 0.1;
    vec3 ambient_light = ambient_strength * light_color * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light_pos - FragPos);

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color * material.diffuse;

    float specular_strength = 0.5;
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shine);

    vec3 specular = specular_strength * spec * light_color * material.specular;

    vec3 result = (ambient_light + diffuse + specular) * model_color;
    FragColor = vec4(result, 1.0);
}