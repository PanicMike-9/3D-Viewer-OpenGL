#pragma once

#include <glm/glm.hpp>

struct DirectLight
{
    glm::vec3 direction {-0.2f, -1.0f, -0.3f};

    glm::vec3 ambient  {0.05f, 0.05f, 0.05f};
    glm::vec3 diffuse  {0.4f, 0.4f, 0.4f};
    glm::vec3 specular {0.5f, 0.5f, 0.5f};
};

struct PointLight
{
    glm::vec3 position {0.0f, 0.0f, 0.0f};

    glm::vec3 ambient  {0.05f, 0.05f, 0.05f};
    glm::vec3 diffuse  {0.8f, 0.8f, 0.8f};
    glm::vec3 specular {1.0f, 1.0f, 1.0f};

    float constant  {1.0f};
    float linear    {0.09f};
    float quadratic {0.032f};
};

struct SpotLight
{
    glm::vec3 position  {0.0f, 0.0f, 0.0f};
    glm::vec3 direction {0.0f, 0.0f, -1.0f};

    glm::vec3 ambient  {0.0f, 0.0f, 0.0f};
    glm::vec3 diffuse  {1.0f, 1.0f, 1.0f};
    glm::vec3 specular {1.0f, 1.0f, 1.0f};
    
    float cut_off       {12.5f};
    float outer_cut_off {15.0f};

    float constant  {1.0f};
    float linear    {0.09f};
    float quadratic {0.032f};
};