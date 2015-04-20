#version 330

smooth in vec4 theColor;
uniform vec4 theOffset;

layout(location = 0) out vec4 color;

void main() {
    color = clamp(theColor + theOffset, 0.0, 1.0);
}
