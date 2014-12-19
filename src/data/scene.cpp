#include "scene.h"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "../math/hypermath.h"

// Renders the objects (and their children, and ...)
// using the provided camera and program.
void Scene::render()
{
    glUseProgram(program);

    glm::dmat4 proj = camera.get_projection();
    glm::dmat4 view = camera.get_view();
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr((glm::mat4)proj));
    
    for(object* o : objects)
    {
        if(o->visible)
        {
            render_object(*o, view);
        }
    }

    glUseProgram(0);
}

void Scene::render_stereo(int textureScale, CameraControls control, GLuint left_framebuffer, GLuint right_framebuffer)
{
    control.move_right(-control.get_ipd() * 0.5f * control.get_meter());

    // Render to our left framebuffer
    glViewport((6.4 * textureScale) * _border, (8 * textureScale) * _border,
        (6.4 * textureScale) * (1.0 - (_border * 2.0)), (8 * textureScale) * (1.0 - (_border * 2.0)));
    glBindFramebuffer(GL_FRAMEBUFFER, left_framebuffer);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    render();

    control.move_right(control.get_ipd() * control.get_meter());

    // Render to our right framebuffer
    glViewport((6.4 * textureScale) * _border, (8 * textureScale) * _border,
        (6.4 * textureScale) * (1.0 - (_border * 2.0)), (8 * textureScale) * (1.0 - (_border * 2.0)));
    glBindFramebuffer(GL_FRAMEBUFFER, right_framebuffer);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    render();

    control.move_right(-control.get_ipd() * 0.5f * control.get_meter());
}

// Renders an object.
// The supplied modelview matrix is the modelview matrix so far, when
// traversing through the object's parents. For example, if the object
// has no parent, it's the view matrix of the camera.
void Scene::render_object(object o, glm::dmat4 modelview)
{
    modelview = modelview * o.transformation;
    glUniformMatrix4fv(glGetUniformLocation(program, "modelview"), 1, GL_FALSE, glm::value_ptr((glm::mat4)modelview));
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

