#pragma once

// C++ libs
#include <vector>
#include <string>

// glm libs
#include <glm/glm.hpp>

// for Shader
#include "shader.hpp"

// organize vertex data
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

// organize texture data
struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        
        // constructor
        Mesh
        (
            std::vector<Vertex> vertices, 
            std::vector<unsigned int> indices, 
            std::vector<Texture> textures
        );

        // draw/render
        void draw(Shader& shader);

    private:
        // render data
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        // set mesh data
        void setup_mesh();
};