#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

struct mesh
{
    GLuint vao;
    GLenum mode;
    GLint first;
    GLsizei count;
};

#endif /* MESH_H */

