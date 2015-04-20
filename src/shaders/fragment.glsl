#version 330

smooth in vec4 theColor;
smooth in vec4 theNormal;

layout(location = 0) out vec4 color;

void main() {
    color = (theColor);
}
