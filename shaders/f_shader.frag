#version 410 core

in vec3 vertexColor;

out vec4 FragColor;

uniform float u_time;

void main()
{
//    FragColor = vec4(
//        abs(sin(u_time)),
//        abs(sin(u_time + 2.0)),
//        abs(sin(u_time + 4.0)),
//        1.0
//    );

    FragColor = vec4(vertexColor, 1.0);
}



