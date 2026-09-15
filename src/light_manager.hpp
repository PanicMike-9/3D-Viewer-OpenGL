#if 0
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

        std::vector<PointLight> point_lights;
        inline void add_point_lights(const PointLight& light)
        {
            point_lights.push_back(light);
        }

        #if 1
        std::vector<SpotLight>  spot_lights;
        inline void add_spot_lights(const SpotLight& light)
        {
            spot_lights.push_back(light);
        }
        #endif

        void update_shader_uniforms(Shader& shader) /* [[maybe_unused]] const Camera& camera */
        {
            shader.use();

            constexpr glm::vec3 light_color {1.0f, 1.0f, 1.0f};

            shader.set_vec3("dir_light.direction", light_color * dir_light.direction);
            shader.set_vec3("dir_light.ambient",   light_color * dir_light.ambient);
            shader.set_vec3("dir_light.diffuse",   light_color * dir_light.diffuse);
            shader.set_vec3("dir_light.specular",  light_color * dir_light.specular);

            // point lights
            shader.set_int("num_point_lights", static_cast<int>(point_lights.size()));

            for (std::size_t i {0}; i < point_lights.size(); ++i)
            {
                std::string base = "point_lights[" + std::to_string(i) + "].";

                shader.set_vec3(base + "position", point_lights[i].position);

                shader.set_vec3(base + "ambient",  point_lights[i].ambient); 
                shader.set_vec3(base + "diffuse",  point_lights[i].diffuse);
                shader.set_vec3(base + "specular", point_lights[i].specular);

                shader.set_float(base + "constant",  point_lights[i].constant);
                shader.set_float(base + "linear",    point_lights[i].linear);
                shader.set_float(base + "quadratic", point_lights[i].quadratic);
            }

            #if 1
            // spot light
            shader.set_int("num_spot_lights", static_cast<int>(spot_lights.size()));

            for (std::size_t i {0}; i < spot_lights.size(); ++i)
            {
                std::string base = "spot_lights[" + std::to_string(i) + "].";

                shader.set_vec3(base + "position", spot_lights[i].position);
                shader.set_vec3(base + "direction", spot_lights[i].direction);

                shader.set_vec3(base + "ambient",  spot_lights[i].ambient); 
                shader.set_vec3(base + "diffuse",  spot_lights[i].diffuse);
                shader.set_vec3(base + "specular", spot_lights[i].specular);

                shader.set_float(base + "constant",  spot_lights[i].constant);
                shader.set_float(base + "linear",    spot_lights[i].linear);
                shader.set_float(base + "quadratic", spot_lights[i].quadratic);

                shader.set_float(base + "cut_off",  glm::cos(glm::radians(spot_lights[i].cut_off))); 
                shader.set_float(base + "outer_cut_off", glm::cos(glm::radians(spot_lights[i].outer_cut_off))); 
            }
            #endif
        }
};
#endif
