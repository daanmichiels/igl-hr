#version 150

//layout (location = 0) in vec4 position;
in vec4 position;
//layout (location = 1) in vec3 color;
in vec3 color;

uniform mat4 projection;
uniform mat4 modelview;

smooth out vec3 theColor;

void main()
{
    gl_Position = projection * modelview * position;
    theColor = color;
}
