#version 330 core

layout(location = 0) in vec3 a_pos;

out vec4 vertex_color;

void main()
{
    gl_position = vec4(a_pos, 1.0);
    vertex_color = vec4(0.5, 0.1, 0.8, 1.0);
}