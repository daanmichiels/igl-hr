#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

//TODO: make all the explanations here doxygen-style
//      actually, do that for the whole project

struct mesh
{
    GLuint vao;
    GLenum mode;
    GLint first;
    GLsizei count;

    /** whether the data of this mesh is stored in ram
     * we want to keep it in ram if we want to deform the mesh every frame on the cpu, for example
     * otherwise, it's ok to just store it on the gpu (in_ram = false)
     */
    bool in_ram = false;
    void *data = nullptr; //if in_ram is true, a pointer to the data
    //I suspect this one can be deduced form first and count
    //TODO: check whether count and data_size are the exact same thing; I think one is a multiple of the other, with factor the size of a vertex; should we store both?
    size_t data_size = 0; //the size of the data in ram, in bytes
};

#endif /* MESH_H */

