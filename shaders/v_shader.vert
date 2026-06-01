#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float xOffset;
uniform float yOffset;

float new_x;
float new_y;

uniform float size;

uniform float angle;

void main()
{
  new_x = aPos.x * cos(angle) - aPos.y * sin(angle);
  new_y = aPos.x * sin(angle) + aPos.y * cos(angle);

  //gl_Position = vec4(new_x * size + xOffset, new_y * size + yOffset, aPos.z, 1.0);
  gl_Position = vec4(new_x * size + xOffset, new_y * size + yOffset, aPos.z, 1.0);
  ourColor = aColor;
}