#pragma once

#include <glad/glad.h> // all OpenGL headers

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
};