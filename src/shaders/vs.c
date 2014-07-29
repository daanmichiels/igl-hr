#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;

smooth out vec3 theColor;

void main()
{
    gl_Position = position;
    theColor = color;
}