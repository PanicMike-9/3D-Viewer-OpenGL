#version 410 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

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

uniform vec3 view_pos;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 model_color = texture(texture_diffuse1, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light.position - FragPos);
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);

    vec3 ambient = light.ambient * model_color;

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * model_color;

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shine);

    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}