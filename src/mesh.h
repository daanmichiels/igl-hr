#ifndef MESH_H
#define MESH_H

struct mesh
{
    GLuint vao;
    GLenum mode;
    GLint first;
    GLsizei count;
    void render()
    {
        glBindVertexArray(vao);
        glDrawArrays(mode, first, count);
    }
};

#endif /* MESH_H */

