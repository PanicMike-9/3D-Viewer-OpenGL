#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 light_color;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 model_color = texture(texture_diffuse1, TexCoords).rgb;

    float ambient_strength = 0.1;
    vec3 ambient_light = ambient_strength * light_color;

    vec3 result = model_color * ambient_light;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(0.5f, 0.0f, 0.5f, 1.0f); // magenta color for debugging
}