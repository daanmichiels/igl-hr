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

