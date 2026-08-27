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

struct DirectLight
{
};
uniform DirectLight dir_light;

struct PointLight
{
};
uniform PointLight point_light;

struct Light 
{
    vec3 position; // for point light
    vec3 direction; // for directional light
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // point light values
    float constant;
    float linear;
    float quadratic;

    // spot light value(s)
    float cut_off;
    float outer_cut_off;
};
uniform Light light;

uniform vec3 view_pos;

void main()
{
    vec4 diff_sample = texture(material.texture_diffuse1, TexCoords);
    vec3 model_color_diff = diff_sample.rgb;
    vec3 model_color_spec = texture(material.texture_specular1, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    // vec3 light_dir = normalize(-light.direction); // directional light
    vec3 light_dir = normalize(light.position - FragPos); // point light
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

    // spot light calculation
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
    
    diffuse  *= intensity;
    specular *= intensity;

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