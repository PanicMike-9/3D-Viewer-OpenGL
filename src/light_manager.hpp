#pragma once

#include <vector>
#include <utility>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "light_structs.hpp"
#include "shader.hpp"
#include "camera.hpp"

struct ManagedPointLight
{
    int id {};
    PointLight pl_data;
};

struct ManagedSpotLight
{
    int id {};
    SpotLight sl_data;
};

class LightManager
{
    public:
        DirectLight dir_light;

        static constexpr std::size_t MAX_SPOT_LIGHTS  {2};
        static constexpr std::size_t MAX_POINT_LIGHTS {4};

        int next_point_light_id {};
        int next_spot_light_id {};

        // ---- point light ----
        std::vector<ManagedPointLight> point_lights;
        inline int add_point_lights(const PointLight& light)
        {
            if (point_lights.size() >= MAX_POINT_LIGHTS)
                return -1;

            int id = next_point_light_id++;
            point_lights.emplace_back(ManagedPointLight{id, light});
            return id;
        }

        // during point light removal order in the array is maintained
        // using point_lights.erase(), but can be expensive for larger sizes
        // as .erase() is O(n)
        inline bool remove_point_light_ordered(int id)
        {
            auto find_id = std::find_if(point_lights.begin(), point_lights.end(), 
            [id](const ManagedPointLight& managed) { return managed.id == id; });

            if (find_id == point_lights.end())
                return false;

            point_lights.erase(find_id);
            return true;
        }

        // unordered removals are better supported for larger sizes
        // as pop_back() is O(1)
        // but because of std::move and .back(), the order is lost
        inline bool remove_point_light_unordered(int id)
        {
            auto find_id = std::find_if(point_lights.begin(), point_lights.end(), 
            [id](const ManagedPointLight& managed) { return managed.id == id; });

            if (find_id == point_lights.end())
                return false;
            
            *find_id = std::move(point_lights.back());
            point_lights.pop_back();
            return true;
        }

        PointLight* get_point_light(int id)
        {
            auto find_id = std::find_if(point_lights.begin(), point_lights.end(),
             [id](const ManagedPointLight& managed) { return managed.id == id; });

            if (find_id == point_lights.end())
                return nullptr;

            return &find_id->pl_data;
        }

        std::vector<std::pair<int, PointLight>> get_all_point_lights() const
        {
            std::vector<std::pair<int, PointLight>> result;

            for (const ManagedPointLight& i : point_lights)
            {
                result.emplace_back(i.id, i.pl_data);
            }

            return result;
        }

        // ---- spot light ----
        std::vector<ManagedSpotLight> spot_lights;
        inline int add_spot_lights(const SpotLight& light)
        {
            if (spot_lights.size() >= MAX_SPOT_LIGHTS)
                return -1;

            int id = next_spot_light_id++;
            spot_lights.emplace_back(ManagedSpotLight{id, light});
            return id;       
        }

        // during spot light removal order in the array is maintained
        // using spot_lights.erase(), but can be expensive for larger sizes
        // as .erase() is O(n)
        inline bool remove_spot_light_ordered(int id)
        {
            auto find_id = std::find_if(spot_lights.begin(), spot_lights.end(),
            [id](const ManagedSpotLight& managed) { return managed.id == id; } );

            if (find_id == spot_lights.end())
                return false;
            
            spot_lights.erase(find_id);
            return true;
        }

        // unordered removals are better supported for larger sizes
        // as pop_back() is O(1)
        // but because of std::move and .back(), the order is lost
        inline bool remove_spot_light_unordered(int id)
        {
            auto find_id = std::find_if(spot_lights.begin(), spot_lights.end(), 
            [id](const ManagedSpotLight& managed) { return managed.id == id; });

            if (find_id == spot_lights.end())
                return false;
            
            *find_id = std::move(spot_lights.back());
            spot_lights.pop_back();
            return true;
        }

        SpotLight* get_spot_light(int id)
        {
            auto find_id = std::find_if(spot_lights.begin(), spot_lights.end(),
                [id](const ManagedSpotLight& managed) { return managed.id == id; });

            if (find_id == spot_lights.end())
                return nullptr;

            return &find_id->sl_data;
        }

        std::vector<std::pair<int, SpotLight>> get_all_spot_lights() const
        {
            std::vector<std::pair<int, SpotLight>> result;

            for (const ManagedSpotLight& i : spot_lights)
            {
                result.emplace_back(i.id, i.sl_data);
            }

            return result;
        }

        void update_shader_uniforms(Shader& shader) /* [[maybe_unused]] const Camera& camera */
        {
            shader.use();

            constexpr glm::vec3 light_color {1.0f, 1.0f, 1.0f};

            shader.set_vec3("dir_light.direction", light_color * dir_light.direction);
            shader.set_vec3("dir_light.ambient",   light_color * dir_light.ambient);
            shader.set_vec3("dir_light.diffuse",   light_color * dir_light.diffuse);
            shader.set_vec3("dir_light.specular",  light_color * dir_light.specular);

            #if 1
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
            #endif

            #if 1
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

                shader.set_float(base + "cut_off",  glm::cos(glm::radians(light.cut_off))); 
                shader.set_float(base + "outer_cut_off", glm::cos(glm::radians(light.outer_cut_off))); 
            }
            #endif
        }
};