#ifndef VERTEX_SHADER
#define VERTEX_SHADER

char _binary_src_shaders_vertex_glsl_start[] = "#version 330\n\nlayout (location = 0) in vec4 position;\nlayout (location = 1) in vec3 color;\nuniform mat4 projection;\nuniform mat4 modelview;\n\nsmooth out vec3 theColor;\nvoid main()\n{\n  gl_Position = projection * modelview * position;\n  theColor = color;\n}";

#endif