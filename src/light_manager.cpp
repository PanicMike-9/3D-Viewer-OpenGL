#include "light_manager.hpp"

PointLight* LightManager::get_point_light(int id)
{
    auto find_id = std::find_if(point_lights.begin(), point_lights.end(),
        [id](const ManagedPointLight& managed) { return managed.id == id; });

    if (find_id == point_lights.end())
        return nullptr;

    return &find_id->pl_data;
}

std::vector<std::pair<int, PointLight>> LightManager::get_all_point_lights() const
{
    std::vector<std::pair<int, PointLight>> result;

    for (const ManagedPointLight& i : point_lights)
    {
        result.emplace_back(i.id, i.pl_data);
    }

    return result;
}


SpotLight* LightManager::get_spot_light(int id)
{
    auto find_id = std::find_if(spot_lights.begin(), spot_lights.end(),
        [id](const ManagedSpotLight& managed) { return managed.id == id; });

    if (find_id == spot_lights.end())
        return nullptr;

    return &find_id->sl_data;
}

std::vector<std::pair<int, SpotLight>> LightManager::get_all_spot_lights() const
{
    std::vector<std::pair<int, SpotLight>> result;

    for (const ManagedSpotLight& i : spot_lights)
    {
        result.emplace_back(i.id, i.sl_data);
    }

    return result;
}

void LightManager::update_shader_uniforms(Shader& shader) /* [[maybe_unused]] const Camera& camera */
{
    shader.use();

    #if 0
    constexpr glm::vec3 light_color {1.0f, 1.0f, 1.0f};

    shader.set_vec3("dir_light.direction", light_color * dir_light.direction);
    shader.set_vec3("dir_light.ambient",   light_color * dir_light.ambient);
    shader.set_vec3("dir_light.diffuse",   light_color * dir_light.diffuse);
    shader.set_vec3("dir_light.specular",  light_color * dir_light.specular);
    #endif

    // point lights
    shader.set_int("num_point_lights", static_cast<int>(point_lights.size()));

    for (std::size_t i {0}; i < point_lights.size(); ++i)
    {
        const PointLight& light = point_lights[i].pl_data;
        std::string base = "point_lights[" + std::to_string(i) + "].";

        shader.set_vec3(base + "position", light.position);

        shader.set_vec3(base + "ambient",  light.ambient); 
        shader.set_vec3(base + "diffuse",  light.diffuse);
        shader.set_vec3(base + "specular", light.specular);

        shader.set_float(base + "constant",  light.constant);
        shader.set_float(base + "linear",    light.linear);
        shader.set_float(base + "quadratic", light.quadratic);
    }

    // spot light
    shader.set_int("num_spot_lights", static_cast<int>(spot_lights.size()));

    for (std::size_t i {0}; i < spot_lights.size(); ++i)
    {
        const SpotLight& light = spot_lights[i].sl_data;
        std::string base = "spot_lights[" + std::to_string(i) + "].";

        shader.set_vec3(base + "position",  light.position);
        shader.set_vec3(base + "direction", light.direction);

        shader.set_vec3(base + "ambient",  light.ambient); 
        shader.set_vec3(base + "diffuse",  light.diffuse);
        shader.set_vec3(base + "specular", light.specular);

        shader.set_float(base + "constant",  light.constant);
        shader.set_float(base + "linear",    light.linear);
        shader.set_float(base + "quadratic", light.quadratic);

        shader.set_float(base + "cut_off",  light.cut_off); 
        shader.set_float(base + "outer_cut_off", light.outer_cut_off); 
    }
}