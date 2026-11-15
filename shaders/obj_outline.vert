#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float outline_thickness;

void main()
{
    vec3 world_normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    vec4 world_pos = model * vec4(aPos, 1.0);

    world_pos.xyz += world_normal * outline_thickness;
    gl_Position = projection * view * world_pos;

    // vec3 extrude_pos = aPos + (aNormal * outline_thickness);
    // vec3 extrude_pos = aPos * (1.0 + outline_thickness);
    // gl_Position = projection * view * model * vec4(extrude_pos, 1.0);
}