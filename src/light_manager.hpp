#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "light_structs.hpp"
#include "shader.hpp"
#include "camera.hpp"

class LightManager
{
    public:
        DirectLight dir_light;
        std::vector<SpotLight>  spot_lights;
        std::vector<PointLight> point_lights;

        inline void add_point_lights(const PointLight& light)
        {
            point_lights.push_back(light);
        }

        inline void add_spot_lights(const SpotLight& light)
        {
            spot_lights.push_back(light);
        }

        void update_shader_uniforms(Shader& shader, [[maybe_unused]] const Camera& camera)
        {
            shader.use();

            constexpr glm::vec3 light_color {1.0f, 1.0f, 1.0f};

            constexpr glm::vec3 ambient_color  {light_color * glm::vec3(0.2f)}; // shadow brightness
            constexpr glm::vec3 diffuse_color  {light_color * glm::vec3(0.8f)}; // direct surface light 
            constexpr glm::vec3 specular_color {light_color * glm::vec3(1.0f)}; // brightness of shine

            shader.set_vec3("dir_light.direction", dir_light.direction);
            shader.set_vec3("dir_light.ambient",   dir_light.ambient);
            shader.set_vec3("dir_light.diffuse",   dir_light.diffuse);
            shader.set_vec3("dir_light.specular",  dir_light.specular);
        }
};
