#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;

// Output data ; will be interpolated for each fragment.
out vec2 theUV;

void main(){
  gl_Position = vec4(position, 1);
  theUV = position.xy;
}