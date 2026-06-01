#pragma once

// all OpenGL headers
#include <glad/glad.h> 

// glm libraries
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        unsigned int ID;

        // constructor builds shader and reads 
        Shader(const char* vertex_path, const char* fragment_path);

        // activate shader
        void use();

        // utility uniform functions
        void set_bool(const std::string &name, bool value) const;
        void set_int(const std::string &name, int value) const;
        void set_float(const std::string &name, float value) const;

        // set matrix 4 for glm
        void set_mat4(const std::string &name, const glm::mat4 &mat) const;
};