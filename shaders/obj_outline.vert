#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float outline_thickness = 0.3;

void main()
{
    vec3 extrude_pos = aPos + (aNormal * outline_thickness);
    gl_Position = projection * view * model * vec4(extrude_pos, 1.0);
}