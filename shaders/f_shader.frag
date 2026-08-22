#version 410 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shine;
};
uniform Material material;

struct Light 
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

uniform vec3 view_pos;

void main()
{
    vec4 diff_sample = texture(material.texture_diffuse1, TexCoords);
    vec3 model_color_diff = diff_sample.rgb;
    vec3 model_color_spec = texture(material.texture_specular1, TexCoords).rgb;


    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(-light.direction); // directional light
    // vec3 light_dir = normalize(light.position - FragPos); // point light
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);

    vec3 ambient = light.ambient * model_color_diff;

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * model_color_diff;

    // backface light leaks protection
    vec3 specular = vec3(0.0);
    if (diff > 0.0)
    {
        float shine_clamp = max(material.shine, 1.0);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shine_clamp);
        specular = light.specular * (spec * model_color_spec);
    }

    // point light calculation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * 
                        (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, diff_sample.a); // .a preserves alpha channel
}