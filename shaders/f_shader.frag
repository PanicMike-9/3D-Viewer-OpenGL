#version 410 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 light_color;
uniform vec3 light_pos;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 model_color = texture(texture_diffuse1, TexCoords).rgb;

    float ambient_strength = 0.1;
    vec3 ambient_light = ambient_strength * light_color;

    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light_pos - FragPos);

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient_light + diffuse) * model_color;
    FragColor = vec4(result, 1.0);
}