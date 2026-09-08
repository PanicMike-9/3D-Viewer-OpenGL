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
        }
};
