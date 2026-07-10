#version 410 core

in vec3 vertexColor;

out vec4 FragColor;

uniform float u_time;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}



