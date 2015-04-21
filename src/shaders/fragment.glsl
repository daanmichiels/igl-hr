#version 330

smooth in vec3 theColor;
smooth in vec4 theNormal;

uniform vec4 lamp;
uniform vec4 camera;
uniform int specular;
uniform int diffuse;
uniform int ambient;
uniform float surfaceSpec;
uniform float surfaceDiff;
uniform float surfaceAmb;

layout(location = 0) out vec3 color;

void main() {
    //color = (theColor);
	color = surfaceAmb*ambient + surfaceDiff*()
}
