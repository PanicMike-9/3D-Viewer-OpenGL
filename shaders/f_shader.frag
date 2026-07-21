#version 410 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diff1;

void main()
{
    FragColor = texture(texture_diff1, TexCoords);
}



