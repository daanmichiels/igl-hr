#ifndef FRAGMENT_SHADER
#define FRAGMENT_SHADER

char _binary_src_shaders_fragment_glsl_start[] = "#version 330\n\nsmooth in vec3 theColor;\n\nlayout(location = 0) out vec3 color;\n\nvoid main() {\n\n    color = (theColor);\n}\n";

#endif