#version 150

smooth in vec3 theColor;

out vec4 finalColor;

void main() {
    //why is this 0.5?
    finalColor = vec4(theColor, 0.5);
}
