#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(0.5f, 0.0f, 0.5f, 1.0f); // magenta color for debugging
}