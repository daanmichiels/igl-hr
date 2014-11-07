#include "scene.h"
#include <GL/glew.h>
#include "../thirdparty/glm/glm/glm.hpp"
#include "../thirdparty/glm/glm/gtc/type_ptr.hpp"

// Renders the objects (and their children, and ...)
// using the provided camera and program.
void Scene::render()
{
    glUseProgram(program);

    glm::mat4 proj = camera.get_projection();
    glm::mat4 view = camera.get_view();
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    
    for(object* o : objects)
    {
        if(o->visible)
        {
            render_object(*o, view);
        }
    }

    glUseProgram(0);
}

// Renders an object.
// The supplied modelview matrix is the modelview matrix so far, when
// traversing through the object's parents. For example, if the object
// has no parent, it's the view matrix of the camera.
void Scene::render_object(object o, glm::mat4 modelview)
{
    modelview = modelview * o.transformation;
    glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    for(mesh m : o.meshes)
    {
        render_mesh(m);
    }
    for(object* child : o.children)
    {
        render_object(*child, modelview);
    }
}

// Renders a mesh.
void Scene::render_mesh(mesh m)
{
    glBindVertexArray(m.vao);
    glDrawArrays(m.mode, m.first, m.count);
}

