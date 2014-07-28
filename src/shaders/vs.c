#version 150

in vec4 position;

void main() {
    // does not alter the vertices at all
    gl_Position = position;
}