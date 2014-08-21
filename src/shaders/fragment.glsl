#version 330

smooth in vec3 theColor;

out vec4 finalColor;

void main() {
    finalColor = vec4(theColor, 0.5);
}