#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;

uniform mat4 projection;
uniform mat4 modelview;

in vec2 UV;
out vec2 theUV;

smooth out vec3 theColor;

void main()
{
    gl_Position = (projection * modelview * position);
    theColor = color;
    theUV = UV;
}
