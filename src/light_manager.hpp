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

        // ---- point light ----
        inline int add_point_light(const PointLight& light);

        inline bool remove_point_light_ordered(int id);
        inline bool remove_point_light_unordered(int id);

        PointLight* get_point_light(int id);
        std::vector<std::pair<int, PointLight>> get_all_point_lights() const;

        // ---- spot light ----
        inline int add_spot_light(const SpotLight& light);

        inline bool remove_spot_light_ordered(int id);
        inline bool remove_spot_light_unordered(int id);

        SpotLight* get_spot_light(int id);
        std::vector<std::pair<int, SpotLight>> get_all_spot_lights() const;

        // ---- update all light uniforms ----
        void update_shader_uniforms(Shader& shader); /* [[maybe_unused]] const Camera& camera */

    private:
        std::vector<ManagedPointLight> point_lights;
        std::vector<ManagedSpotLight> spot_lights;

        int next_point_light_id {};
        int next_spot_light_id {};

        static constexpr std::size_t MAX_SPOT_LIGHTS  {2};
        static constexpr std::size_t MAX_POINT_LIGHTS {4};
};

#include "light_manager.inl"