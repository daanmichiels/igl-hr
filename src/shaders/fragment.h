#ifndef FRAGMENT_SHADER
#define FRAGMENT_SHADER

char _binary_src_shaders_fragment_glsl_start[] = "#version 330\n\nsmooth in vec3 theColor;\n\nout vec4 finalColor;\n\nvoid main() {\n    finalColor = vec4(theColor, 0.5);\n}\n";

#endif