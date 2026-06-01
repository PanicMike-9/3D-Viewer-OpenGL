#include "shader.hpp"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    std::string vertex_code;
    std::string fragment_code;

    std::ifstream v_shader_file; // vertex shader file
    std::ifstream f_shader_file; // fragment shader file

    // ifstream obj throw exceptions
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // try and catch
    try 
    {
        // open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);

        std::stringstream v_shader_stream; // vertex shader stream
        std::stringstream f_shader_stream; // fragment shader stream

        // read from shader file, into stream
        v_shader_stream << v_shader_file.rdbuf(); 
        f_shader_stream << f_shader_file.rdbuf(); 

        // close opened files(very important)
        v_shader_file.close();
        f_shader_file.close();

        // convert stream into string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    // into c-style string
    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();

    // compile shaders
     unsigned int vertex, fragment;
     int success;
     char info_log[512];

     // vertex shader
     vertex = glCreateShader(GL_VERTEX_SHADER);
     glShaderSource(vertex, 1, &v_shader_code, NULL);
     glCompileShader(vertex);

     // print vertex compile errors
     glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

     if(!success)
     {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
     }

     // fragment shader
     fragment = glCreateShader(GL_FRAGMENT_SHADER);
     glShaderSource(fragment, 1, &f_shader_code, NULL);
     glCompileShader(fragment);

     // print fragment compiler errors
     glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

     if(!success)
     {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
     }

     // create shader program
     ID = glCreateProgram();
     glAttachShader(ID, vertex);
     glAttachShader(ID, fragment);
     glLinkProgram(ID);

     // linking errors
     glGetProgramiv(ID, GL_LINK_STATUS, &success);

     // print error logs
     if(!success)
     {
        glGetProgramInfoLog(ID, 512, NULL, info_log);
        std::cout  << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
     }

     // delete shaders they aren't nescessary
     glDeleteShader(vertex);
     glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}