#version 410 core

out vec4 outColor;

uniform float u_time;

void main()
{
  outColor = vec4(
    abs(sin(u_time)), 
    abs(sin(u_time + 2.0)),
    abs(sin(u_time + 4.0)),
    1.0
  );
}


