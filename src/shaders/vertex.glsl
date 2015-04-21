#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec4 normal;

uniform mat4 projection;
uniform mat4 modelview;

smooth out vec3 theColor;
smooth out vec4 theNormal;

void main()
{
    gl_Position = (projection * modelview * position);
    theColor = color;
	theNormal = normal;
}
