#version 330

smooth in vec3 theColor;
uniform vec3 theOffset;

layout(location = 0) out vec3 color;

void main() {
    color = clamp(theColor + theOffset, 0.0, 1.0);
}
